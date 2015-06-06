#include "JasonQt_TcpClient.h"

using namespace JasonQt_Tcp;

// ClientSocket
ClientSocket::ClientSocket(const QString &connectAddress, const quint16 &connectPort, const SocketSettings &socketSettings):
    Socket(socketSettings),
    m_connectAddress(connectAddress),
    m_connectPort(connectPort)
{ }

bool ClientSocket::readConnect(QTcpSocket *socket)
{
    socket->connectToHost(m_connectAddress, m_connectPort);

#ifndef Q_OS_WINPHONE
    return socket->waitForConnected(m_socketSettings.connectWaitTime());
#else
    return true;
#endif
}

// ClientStart
ClientStart::ClientStart(ReadySend *readySend):
    m_readySend(readySend)
{
    connect(this, SIGNAL(signalsSend(QByteArray,quint32)), this, SLOT(slotsSend(QByteArray,quint32)));
}

void ClientStart::send(const QByteArray &data, const quint32 &flag)
{
    if(!ClientManage::m_exitByApplication)
    {
        emit signalsSend(data, flag);
    }
}

void ClientStart::slotsSend(const QByteArray &data, const quint32 &flag)
{
    m_readySend->readySend(data, flag);
}

// ClientManage
bool ClientManage::m_exitByApplication = false;

ClientManage::ClientManage(const ClientSettings &clientSettings, const SocketSettings &socketSettings):
    m_clientSettings(clientSettings),
    m_socketSettings(socketSettings)
{
    if(m_clientSettings.workOnSingleThread())
    {
        this->start();
    }
    else
    {
        m_start = new ClientStart(this);
        m_isRun = true;
    }
}

ClientManage::~ClientManage(void)
{
    if(m_clientSettings.workOnSingleThread())
    {
        this->quit();
        this->wait();
    }
    else
    {
        delete m_start;
        m_start = NULL;

        for(auto now = m_sockets.begin(); now != m_sockets.end(); now++)
        {
            now.key()->deleteLater();
        }
    }
}

void ClientManage::send(const QByteArray &send)
{
    this->waitForRun();

    m_start->send(send, 0);
}

bool ClientManage::send(const QByteArray &send, QByteArray &accepted)
{
    this->waitForRun();

    QEventLoop eventLoop;
    quint32 currentRunFlag = this->getRunFlag();
    bool doneFlag = false;

    m_start->send(send, currentRunFlag);

    auto c = connect(this, &ClientManage::finished, [&](const quint32 &flag, const QByteArray &data, const bool &done)
    {
        if(flag != currentRunFlag)
        {
            return;
        }

        doneFlag = done;
        if(doneFlag)
        {
            accepted = data;
        }

        eventLoop.quit();
    });

    eventLoop.exec();
    disconnect(c);

    return doneFlag;
}

void ClientManage::send(Jdn::Set &send)
{
    if(m_clientSettings.jdeUse())
    {
        send.setJdeKey(m_clientSettings.jdeKey());
    }
    else
    {
        send.setJdeDisabled();
    }

    QByteArray buf(send.toStreamWithSizeFlag());

    if(!m_socketSettings.dataSendStartFlag().isEmpty())
    {
        buf.push_front(m_socketSettings.dataSendStartFlag());
    }
    if(!m_socketSettings.dataSendEndFlag().isEmpty())
    {
        buf.push_back(m_socketSettings.dataSendEndFlag());
    }

    this->send(buf);
}

bool ClientManage::send(Jdn::Set &send, Jdn::Set &accepted)
{
    if(m_clientSettings.jdeUse())
    {
        send.setJdeKey(m_clientSettings.jdeKey());
        accepted.setJdeKey(m_clientSettings.jdeKey());
    }
    else
    {
        send.setJdeDisabled();
        accepted.setJdeDisabled();
    }

    QByteArray buf(send.toStreamWithSizeFlag());

    if(!m_socketSettings.dataSendStartFlag().isEmpty())
    {
        buf.push_front(m_socketSettings.dataSendStartFlag());
    }
    if(!m_socketSettings.dataSendEndFlag().isEmpty())
    {
        buf.push_back(m_socketSettings.dataSendEndFlag());
    }

    if(!this->send(buf, buf))
    {
        return false;
    }

    return accepted.fromStreamWithSizeFlag(buf);;
}

void ClientManage::closeAllConnect(void)
{
    for(auto now = m_sockets.begin(); now != m_sockets.end(); now++)
    {
        now.key()->deleteLater();
    }
    m_sockets.clear();
}

void ClientManage::readySend(const QByteArray &data, const quint32 &flag)
{
    ClientSocket *socket = NULL;

    for(auto now = m_sockets.begin(); now != m_sockets.end(); now++)
    {
        if(!now.value())
        {
            socket = (ClientSocket *)now.key();
        }
    }

    if(!socket && (m_sockets.size() <= m_clientSettings.connectCount()))
    {
        for(int now = 0; now < m_clientSettings.connectTryMax(); now++)
        {
            socket = new ClientSocket(m_clientSettings.connectAddress(), m_clientSettings.connectPort(), m_socketSettings);

            connect(socket, SIGNAL(accepted(Socket*,QByteArray)),           this, SLOT(accepted(Socket*,QByteArray)), Qt::DirectConnection);
            connect(socket, SIGNAL(disconnected(Socket*,SocketExitFlag,int)), this, SLOT(disconnected(Socket*)), Qt::DirectConnection);

            if(socket->begin())
            {
                break;
            }
            delete socket;
            socket = NULL;
        }
    }

    if(!socket)
    {
        qDebug("ClientManage: No more available client");
        if(flag)
        {
            emit finished(flag, QByteArray(), false);
        }
    }
    else
    {
        socket->send(data);
        m_sockets[socket] = flag;
    }
}

void ClientManage::disconnected(Socket *socket)
{
    if(m_sockets[socket])
    {
        emit finished(m_sockets[socket], QByteArray(), false);
    }
    m_sockets.erase(m_sockets.find(socket));
}

void ClientManage::accepted(Socket *socket, const QByteArray &data)
{
    auto it = m_sockets.find(socket);
    if(it != m_sockets.end())
    {
        if(it.value())
        {
            emit finished(it.value(), data, true);
            it.value() = 0;
        }
        else
        {
            if(m_inspectionexceptionWithByteArray)
            {
                (*m_inspectionexceptionWithByteArray)(data, socket);
            }
            else
            {
                Jdn::Set accepted;

                if(m_clientSettings.jdeUse())
                {
                    accepted.setJdeKey(m_clientSettings.jdeKey());
                }
                else
                {
                    accepted.setJdeDisabled();
                }

                if(!accepted.fromStreamWithSizeFlag(data))
                {
                    qDebug() << Jdn::Set::checkStreamSize(data);
                    qDebug() << "ClientManage::Accepted: Have some data need inspection. Data fromStreamWithSizeFlag fail";
                    return;
                }

                if(m_inspectionExceptionWithNodeControl)
                {
                    (*m_inspectionExceptionWithNodeControl)(accepted, socket);
                }
                else
                {
                    while(accepted.next())
                    {
                        qDebug() << QString("ClientManage::Accepted: Have node(Num:%1) need inspection, Data inspection is empty.").arg(accepted.currentNodeNum());
                    }
                }
            }
        }
    }
}

void ClientManage::run(void)
{
    auto o = connect(qApp, &QCoreApplication::aboutToQuit, [&]()
    {
        m_exitByApplication = true;
        this->quit();
    });

    m_start = new ClientStart(this);
    m_isRun = true;

    this->exec();

    delete m_start;
    m_start = NULL;

    for(auto now = m_sockets.begin(); now != m_sockets.end(); now++)
    {
        now.key()->deleteLater();
    }

    if(!m_exitByApplication)
    {
        QEventLoop eventLoop;
        while(eventLoop.processEvents(QEventLoop::ExcludeUserInputEvents));
    }

    disconnect(o);
}

quint32 ClientManage::getRunFlag(void)
{
    m_mutex.lock();
    const auto runFlag = m_currentRunFlag++;
    m_mutex.unlock();
    return runFlag;
}

void ClientManage::waitForRun(void)
{
    if(!m_isRun)
    {
        while(!m_isRun)
        {
            QThread::msleep(1);
        }
        QThread::msleep(50);
    }
}

// Function define
QString JasonQt_Tcp::standardHandleProcess(ClientManage &client,
                                           Jdn::Set &send,
                                           Jdn::Set &accepted,
                                           const int32_t &OKFlag,
                                           const int32_t &FailFlag,
                                           const bool &inspectionFirstNode)
{
    if(!client.send(send, accepted))
    {
        return "CanNotConnect";
    }
    if(accepted.isEmpty())
    {
        return "EmptyAccepted";
    }

    if(inspectionFirstNode)
    {
        if(accepted.currentNodeNum() != OKFlag)
        {
            if(accepted.currentNodeType() != Jdn::WithStringNode)
            {
                return "DifferentNodeType";
            }
            if(accepted.currentNodeNum() != FailFlag)
            {
                return "DifferentFailFlag";
            }
            return accepted.currentNodeQString();
        }

        accepted.pop_front();
    }

    return "OK";
}

QString JasonQt_Tcp::standardHandleProcess(ClientManage &client, Jdn::Set &send,
                                           const int32_t &OKFlag, const int32_t &FailFlag)
{
    Jdn::Set accepted;
    return standardHandleProcess(client, send, accepted, OKFlag, FailFlag);
}


QString JasonQt_Tcp::standardHandleProcess(ClientManage &client, const Jdn::Node &node, const int32_t &OKFlag, const int32_t &FailFlag)
{
    Jdn::Set send;
    send.push_back(node);
    return standardHandleProcess(client, send, OKFlag, FailFlag);
}
