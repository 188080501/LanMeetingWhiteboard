#include "JasonQt_TcpSocket.h"

using namespace JasonQt_Tcp;

// Socket
Socket::Socket(const SocketSettings &socketSettings):
    m_socketSettings(socketSettings)
{
    static bool Flag = true;
    if(Flag)
    {
        Flag = false;
        qRegisterMetaType<SocketExitFlag>("SocketExitFlag");
    }
}

Socket::~Socket(void)
{
    if(m_sendBuffer)
    {
        delete m_sendBuffer;
    }
    if(m_connectMaxTimeTimer)
    {
        delete m_connectMaxTimeTimer;
    }
    if(m_connectMaxReadTimeTimer)
    {
        delete m_connectMaxReadTimeTimer;
    }
    if(m_socket)
    {
        auto socket = m_socket;
        m_socket = NULL;
        socket->disconnectFromHost();
        delete socket;
    }
}

bool Socket::begin(void)
{
    m_socket = new QTcpSocket;

    connect(m_socket, SIGNAL(readyRead()),                          this, SLOT(readyRead()), Qt::DirectConnection);
    connect(m_socket, SIGNAL(bytesWritten(qint64)),                 this, SLOT(bytesWritten(qint64)), Qt::DirectConnection);
    connect(m_socket, SIGNAL(disconnected()),                       this, SLOT(socketDisconnected()), Qt::DirectConnection);
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),  this, SLOT(socketError(QAbstractSocket::SocketError)), Qt::DirectConnection);
    connect(this,     SIGNAL(disconnected(Socket*,SocketExitFlag,int)), this, SLOT(readyClose()));

    m_connectMaxTimeTimer = new QTimer;
    m_connectMaxTimeTimer->setTimerType(Qt::VeryCoarseTimer);
    connect(m_connectMaxTimeTimer,     SIGNAL(timeout()),           this, SLOT(connectMaxTimeout()), Qt::DirectConnection);

    m_connectMaxReadTimeTimer = new QTimer;
    m_connectMaxReadTimeTimer->setTimerType(Qt::VeryCoarseTimer);
    connect(m_connectMaxReadTimeTimer, SIGNAL(timeout()),           this, SLOT(connectMaxReadTimeout()), Qt::DirectConnection);

    connect(this, SIGNAL(needToSend(QByteArray)),                   this, SLOT(readySend(QByteArray)));

    if(!this->readConnect(m_socket))
    {
        this->readyClose();
        return false;
    }

    if(m_socketSettings.connectMaxTime())
    {
        m_connectMaxTimeTimer->start(m_socketSettings.connectMaxTime());
    }
    m_connectMaxReadTimeTimer->start(m_socketSettings.connectMaxReadTime());

    m_connectSartTime = QDateTime::currentDateTime().toTime_t();
    emit conneced(this);

    return true;
}

void Socket::close(void)
{
    emit disconnected(this, ExitFlag_ActiveExit, QDateTime::currentDateTime().toTime_t() - m_connectSartTime);
}

void Socket::send(const QByteArray &data)
{
    if(QThread::currentThread() != m_socket->thread())
    {
        emit needToSend(data);
    }
    else
    {
        this->readySend(data);
    }
}

void Socket::readyRead(void)
{
    m_connectMaxReadTimeTimer->stop();
    m_connectMaxReadTimeTimer->start(m_socketSettings.connectMaxReadTime());

    if(m_socket)
    {
        QByteArray buf;
        while(!m_socket->atEnd())
        {
            buf.append(m_socket->readAll());
        }
        m_acceptedBuffer.append(buf);

        if(buf.size() > m_socketSettings.dataMaxBufferSize())
        {
            emit disconnected(this, ExitFlag_AcceptedBufferError, QDateTime::currentDateTime().toTime_t() - m_connectSartTime);
            return;
        }

        if(m_socketSettings.dataUseAcceptedCheck())
        {
            int flag;
            do
            {
                flag = m_socketSettings.dataAcceptedCheck()(m_acceptedBuffer, buf);
                switch(flag)
                {
                    case -2: { return; }
                    case -1:
                    {
                        emit accepted(this, m_acceptedBuffer);
                        m_acceptedBuffer.clear();
                        break;
                    }
                    default:
                    {
                        if(flag >= 0)
                        {
                            emit accepted(this, m_acceptedBuffer.mid(0, flag));
                            m_acceptedBuffer.remove(0, flag);
                            break;
                        }
                    }
                    case -3: { emit disconnected(this, ExitFlag_AcceptedBufferError, QDateTime::currentDateTime().toTime_t() - m_connectSartTime); }
                }
            }
            while(flag >= 0);
        }
        else
        {
            emit accepted(this, m_acceptedBuffer);
            m_acceptedBuffer.clear();
        }
    }
}

void Socket::bytesWritten(const qint64 &bytes)
{
    if(m_socket)
    {
        m_needToWritten -= bytes;
        if(!m_needToWritten && m_sendBuffer)
        {
            m_needToWritten += m_sendBuffer->begin()->size();
            m_socket->write(*m_sendBuffer->begin());

            m_sendBuffer->pop_front();
            if(m_sendBuffer->isEmpty())
            {
                delete m_sendBuffer;
                m_sendBuffer = NULL;
            }
        }
    }
}

void Socket::connectMaxTimeout(void)
{
    emit disconnected(this, ExitFlag_ConnectMaxTimeout, QDateTime::currentDateTime().toTime_t() - m_connectSartTime);
}

void Socket::connectMaxReadTimeout(void)
{
    emit disconnected(this, ExitFlag_ConnectMaxReadTimeout, QDateTime::currentDateTime().toTime_t() - m_connectSartTime);
}

void Socket::socketDisconnected(void)
{
    emit disconnected(this, ExitFlag_SocketDisconnected, QDateTime::currentDateTime().toTime_t() - m_connectSartTime);
}

void Socket::socketError(const QAbstractSocket::SocketError &error)
{
    if(error == QAbstractSocket::SocketTimeoutError) { return; }

    emit disconnected(this, ExitFlag_SocketError, QDateTime::currentDateTime().toTime_t() - m_connectSartTime);
}

void Socket::readyClose(void)
{
    if(m_socket)
    {
        disconnect(m_socket, SIGNAL(readyRead()),                          this, SLOT(readyRead()));
        disconnect(m_socket, SIGNAL(bytesWritten(qint64)),                 this, SLOT(bytesWritten(qint64)));
        disconnect(m_socket, SIGNAL(disconnected()),                       this, SLOT(socketDisconnected()));
        disconnect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),  this, SLOT(socketError(QAbstractSocket::SocketError)));
        disconnect(this,     SIGNAL(disconnected(Socket*,SocketExitFlag,int)), this, SLOT(readyClose()));

        disconnect(m_connectMaxTimeTimer,     SIGNAL(timeout()),           this, SLOT(connectMaxTimeout()));
        disconnect(m_connectMaxReadTimeTimer, SIGNAL(timeout()),           this, SLOT(connectMaxReadTimeout()));

        disconnect(this,     SIGNAL(disconnected(Socket*,SocketExitFlag,int)), this, SLOT(readyClose()));

        m_socket->deleteLater();
        m_socket = NULL;
        this->deleteLater();
    }
}

void Socket::readySend(const QByteArray &data)
{
    if(m_socket)
    {
        int index = 0;
        if(!m_sendBuffer)
        {
            if(data.size() > m_socketSettings.dataSendMaxSize())
            {
                index = m_socketSettings.dataSendMaxSize();
                m_needToWritten += m_socketSettings.dataSendMaxSize();
                m_socket->write(data.data(), m_socketSettings.dataSendMaxSize());
            }
            else
            {
                index = -1;
                m_needToWritten += data.size();
                m_socket->write(data);
            }
        }
        if(index != -1)
        {
            if(!m_sendBuffer)
            {
                m_sendBuffer = new QList<QByteArray>;
            }
            for(; (index + m_socketSettings.dataSendMaxSize()) <= data.size(); index += m_socketSettings.dataSendMaxSize())
            {
                m_sendBuffer->push_back(QByteArray(data.data() + index, m_socketSettings.dataSendMaxSize()));
            }
            if(index != data.size())
            {
                m_sendBuffer->push_back(QByteArray(data.data() + index, data.size() - index));
            }
        }
    }
}
