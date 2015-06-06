#include "JasonQt_Lan.h"

using namespace JasonQt_Lan;

// LanSearcher
LanSearcher::LanSearcher(const quint16 &port, const int &triggeredInterval, const int timeout, const QString &jdeKey):
    m_port(port),
    m_triggeredInterval(triggeredInterval),
    m_timeout(timeout),
    m_jdeKey(jdeKey)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTriggered()));
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(onAccepted()));
}

void LanSearcher::foreachAvailableIP(const std::function<void (const QHostAddress &)> &foreachIP)
{
    for(auto it = m_availableIp.begin(); it != m_availableIp.end(); it++)
    {
        foreachIP(QHostAddress(it.key()));
    }
}

bool LanSearcher::begin(const bool &printMessage)
{
    m_currentAddressEntry = JasonQt_Net::getNetworkAddressEntry();
    if(printMessage)
    {
        if(m_currentAddressEntry.ip().isNull())
        {
            qDebug() << "Get network address entry fail";
            return false;
        }

        qDebug() << "LanSearcher::begin: Get network address entry succeed, IP:"
                 << m_currentAddressEntry.ip().toString()
                 << ", broadcast:"
                 << m_currentAddressEntry.broadcast().toString();
    }

    m_jdeKey.updatePrivateKey(m_currentAddressEntry.broadcast().toString().toLatin1().data());

    const auto &&flag = m_socket.bind(QHostAddress::Any, m_port);
    if(printMessage)
    {
        if(!flag)
        {
            qDebug() << "LanSearcher::begin: Bind fail";
            return false;
        }

        qDebug() << "LanSearcher::begin: Bind succeed";
    }

    m_timer.start(m_triggeredInterval);

    return true;
}

void LanSearcher::ping(void)
{
    // Check available ip
    const auto &&currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    for(auto it = m_availableIp.begin(); it != m_availableIp.end(); )
    {
        if((currentTime - it.value()) > m_timeout)
        {
            qDebug() << "Dis connect:" << QHostAddress(it.key()).toString();
            emit disConnect(QHostAddress(it.key()));

            m_availableIp.erase(it);
            it = m_availableIp.begin();

            continue;
        }

        it++;
    }

    // Send ping
    Jdn::Set pingData;
    pingData.setJdeKey(m_jdeKey);

    pingData.push_back((int32_t)m_currentAddressEntry.ip().toIPv4Address(), true);

    if(m_onSendCallback)
    {
        (*m_onSendCallback)(pingData);
    }

    auto *socket = new QUdpSocket;
    socket->writeDatagram(pingData.toStreamWithSizeFlag(), m_currentAddressEntry.broadcast(), m_port);

    QTimer::singleShot(2000, socket, SLOT(deleteLater()));
}

void LanSearcher::onTriggered(void)
{
    this->ping();
}

void LanSearcher::onAccepted(void)
{
    while(m_socket.hasPendingDatagrams())
    {
        Jdn::Set data;
        QByteArray buf;

        data.setJdeKey(m_jdeKey);

        buf.resize(m_socket.pendingDatagramSize());
        m_socket.readDatagram(buf.data(), buf.size());

        if(data.fromStreamWithSizeFlag(buf))
        {
            if(data.size() && (data.currentNodeType() == Jdn::OnlyNumNode))
            {
                if(!m_availableIp.contains(quint32(data.currentNodeNum())))
                {
                    qDebug() << "New connect:" << QHostAddress(quint32(data.currentNodeNum())).toString();
                    emit newConnect(QHostAddress(quint32(data.currentNodeNum())));
                }
                else
                {
//                    qDebug() << "Ping from:" << QHostAddress(quint32(data.currentNodeNum())).toString();
                }
                m_availableIp[quint32(data.currentNodeNum())] = QDateTime::currentDateTime().toMSecsSinceEpoch();

                data.pop_front();

                if(m_onAcceptedCallback)
                {
                    (*m_onAcceptedCallback)(data);
                }
            }
        }
    }
}

// LanSocket
LanSocket::LanSocket(LanSearcher *lanSearcher, const quint16 &port, const int &maxThreadCount):
    m_lanSearcher(lanSearcher),
    m_port(port)
{
    m_sendThreadPool.setMaxThreadCount(maxThreadCount);
    m_acceptedThreadPool.setMaxThreadCount(maxThreadCount);

    connect(&m_listener, &TcpListener::newConnect, [&](const qintptr &socketDescriptor)
    {
        QtConcurrent::run(&m_acceptedThreadPool, [=]()
        {
            QTcpSocket socket;
            socket.setSocketDescriptor(socketDescriptor);

            if(!socket.waitForConnected(5000)) { return; }

            this->accepted(socket, true);
        });
    });
}

bool LanSocket::begin(void)
{
    return m_listener.listen(QHostAddress::Any, m_port);
}

void LanSocket::send(Jdn::Set &data)
{
    QByteArray buf(data.toStreamWithSizeFlag());
    m_lanSearcher->foreachAvailableIP([&](const QHostAddress &address)
    {
        this->send(address, buf);
    });
}

void LanSocket::send(Jdn::Set &data, const QList<QHostAddress> &address)
{
    QByteArray buf(data.toStreamWithSizeFlag());
    for(const auto &now: address)
    {
        this->send(now, buf);
    };
}

void LanSocket::send(const QHostAddress &address, const QByteArray &data)
{
    QtConcurrent::run(&m_sendThreadPool, [=]()
    {
        QTcpSocket socket;

        QEventLoop eventLoop;
        QTimer timer;

        QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
        QObject::connect(&socket, SIGNAL(disconnected()), &eventLoop, SLOT(quit()));
        QObject::connect(&socket, &QIODevice::readyRead, [&]()
        {
            this->accepted(socket, false);
            eventLoop.quit();
        });

        socket.connectToHost(address, m_port);

        if(!socket.waitForConnected(5000)) { return; }

        socket.write(data);
        if(!socket.waitForBytesWritten(5000)) { return; }

        timer.setSingleShot(true);
        timer.start(5000);

        eventLoop.exec();
    });
}

void LanSocket::accepted(QTcpSocket &socket, const bool &needReply)
{
    QByteArray buf;
    while(socket.waitForReadyRead(5000))
    {
        buf.append(socket.readAll());
        if(Jdn::Set::checkStreamSize(buf) == Jdn::StreamSizeJustFine)
        {
            Jdn::Set accepted, send;
            accepted.fromStreamWithSizeFlag(buf);
            if(m_onAcceptedCallback)
            {
                (*m_onAcceptedCallback)(accepted, send);
            }

            if(send.size())
            {
                if(needReply)
                {
                    const auto &&buf = send.toStreamWithSizeFlag();
                    socket.write(buf);
                    socket.waitForBytesWritten(5000);
                    socket.waitForDisconnected(5000);
                }
                else
                {
                    qDebug() << "Dont need reply, But have data";
                }
            }
            break;
        }
    }
}
