#include "JasonQt_TcpServer.h"

using namespace JasonQt_Tcp;

// ServerSocket
ServerSocket::ServerSocket(qintptr socketDescriptor, const SocketSettings &socketSettings):
    Socket(socketSettings),
    m_socketDescriptor(socketDescriptor)
{ }

bool ServerSocket::readConnect(QTcpSocket *socket)
{
    if(!socket->setSocketDescriptor(m_socketDescriptor)) { return false; }
    return socket->waitForConnected(m_socketSettings.connectWaitTime());
}

// ServerStart
ServerStart::ServerStart(void)
{
    connect(this, SIGNAL(signalNewConnect(ServerSocket*)), this, SLOT(slotNewConnect(ServerSocket*)));
}

void ServerStart::newConnect(ServerSocket *socket)
{
    emit signalNewConnect(socket);
}

void ServerStart::slotNewConnect(ServerSocket *socket)
{
    socket->begin();
}

// Server
void Server::newConnect(ServerSocket *socket)
{
    connect(socket, SIGNAL(conneced(Socket*)), this, SLOT(connected(Socket*)));
    connect(socket, SIGNAL(disconnected(Socket*,SocketExitFlag,int)), this, SLOT(disconnected(Socket*)));
    m_start->newConnect(socket);
}

void Server::connected(Socket *socket)
{
    m_sockets.insert(socket);
}

void Server::disconnected(Socket *socket)
{
    m_sockets.erase(m_sockets.find(socket));
}

void Server::run(void)
{
    m_start = new ServerStart;
    this->exec();
    delete m_start;
    m_start = NULL;
}

// ServerListen
void ServerListen::incomingConnection(qintptr socketDescriptor)
{
    emit newConnect(socketDescriptor);
}

// ServerManage
ServerManage::ServerManage(const ServerSettings &serverSettings, const SocketSettings &socketSettings):
    m_serverSettings(serverSettings),
    m_socketSettings(socketSettings)
{
    this->start();

    for(auto count = serverSettings.connectThreadMaxCount(); count--; )
    {
        auto server = new Server;
        m_servers.push_back(server);
        server->start();
    }
}

ServerManage::~ServerManage(void)
{
    this->quit();
    this->wait();

    for(auto &now: m_servers)
    {
        now->quit();
        now->wait();
    }
    qDeleteAll(m_servers);
}

int ServerManage::socketCount(void)
{
    int count = 0;
    for(const auto now: m_servers)
    {
        count += now->socketCount();
    }
    return count;
}

void ServerManage::setInspection(const int32_t &num, const std::function<void (Jdn::Set &, Jdn::Set &, Socket *)> &inspection)
{
    if(!m_inspectionWithNum) { m_inspectionWithNum = new QHash<int32_t, std::function<void(Jdn::Set &, Jdn::Set &, Socket *)> >; }

    if(m_inspectionWithNum->contains(num))
    {
        qDebug("ServerManage::setInspection: Num is same");
    }

    m_inspectionWithNum->insert(num, inspection);
}

quint32 ServerManage::howLongBeforeStart(void) const
{
    return QDateTime::currentDateTime().toTime_t() - m_startTime;
}

QString ServerManage::connectTimeToString(const int &connectTime)
{
    if(connectTime < 3600) { return QTime(0, 0).addSecs(connectTime).toString("mm:ss"); }
    else                   { return QTime(0, 0).addSecs(connectTime).toString("hh:mm:ss"); }
}

void ServerManage::run(void)
{
    ServerListen listen;
    connect(&listen, SIGNAL(newConnect(qintptr)), this, SLOT(newConnect(qintptr)), Qt::DirectConnection);

    if(!listen.listen(QHostAddress::AnyIPv4, m_serverSettings.connectPort()))
    {
        qDebug("ServerManage: Cant not listen on %d", m_serverSettings.connectPort());
        return;
    }
    this->exec();
}

void ServerManage::newConnect(const qintptr &socketDescriptor)
{
    int count = (*m_servers.begin())->socketCount();
    Server *server = (*m_servers.begin());

    for(auto now = m_servers.begin() + 1; now != m_servers.end(); now++)
    {
        if((*now)->socketCount() < count)
        {
            count = (*now)->socketCount();
            server = *now;
        }
    }

    if(server->socketCount() < m_serverSettings.connectSingleThreadMaxCount())
    {
        auto socket = new ServerSocket(socketDescriptor, m_socketSettings);
        socket->moveToThread(server->socketThread());

        connect(socket, SIGNAL(accepted(Socket*,QByteArray)),           this, SLOT(accepted(Socket*,QByteArray)), Qt::DirectConnection);
        connect(socket, SIGNAL(conneced(Socket*)),                      this, SLOT(slotConnected(Socket*)), Qt::DirectConnection);
        connect(socket, SIGNAL(disconnected(Socket*,SocketExitFlag,int)), this, SLOT(slotDisconnected(Socket*,SocketExitFlag,int)), Qt::DirectConnection);

        server->newConnect(socket);
    }
    else
    {
        qDebug("ServerManage: No more available server");
    }
}

void ServerManage::accepted(Socket *socket, const QByteArray &data)
{
    if(m_inspectionWithByteArray)
    {
        QByteArray buf;
        if((*m_inspectionWithByteArray)(data, buf, socket))
        {
            socket->send(buf);
        }
    }
    else
    {
        Jdn::Set accepted, send;

        if(m_serverSettings.jdeUse())
        {
            accepted.setJdeKey(m_serverSettings.jdeKey());
            send.setJdeKey(m_serverSettings.jdeKey());
        }
        else
        {
            accepted.setJdeDisabled();
            send.setJdeDisabled();
        }

        if(!accepted.fromStreamWithSizeFlag(data)) { return; }

        if(m_inspectionWithNodeControl)
        {
            (*m_inspectionWithNodeControl)(accepted, send, socket);
        }
        else
        {
            while(accepted.size())
            {
                accepted.readyNext();

                if(m_inspectionWithNum)
                {
                    const auto now = m_inspectionWithNum->find(accepted.currentNodeNum());
                    if(now != m_inspectionWithNum->end())
                    {
                        now.value()(accepted, send, socket);
                        accepted.pop_front();
                        continue;
                    }
                }

                qDebug() << QString("ServerManage::Accepted: Have node(Num:%1) need inspection, Buf inspection is empty.").arg(accepted.currentNodeNum());
                accepted.pop_front();
            }
        }

        if(send.size())
        {
            QByteArray buf(send.toStreamWithSizeFlag());

            if(!m_socketSettings.dataSendStartFlag().isEmpty())
            {
                buf.push_front(m_socketSettings.dataSendStartFlag());
            }
            if(!m_socketSettings.dataSendEndFlag().isEmpty())
            {
                buf.push_back(m_socketSettings.dataSendEndFlag());
            }

            socket->send(buf);
        }
    }
}

void ServerManage::slotConnected(Socket *socket)
{
    emit connected(socket);
}

void ServerManage::slotDisconnected(Socket *socket, const SocketExitFlag &exitFlag, const int &connectTime)
{
    emit disconnected(socket, exitFlag, connectTime);
}
