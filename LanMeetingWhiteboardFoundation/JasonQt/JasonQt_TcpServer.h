/*
    JasonQt_TcpServer

    Copyright: Jason

    Contact email: 188080501@qq.com

    GNU Lesser General Public License Usage
    Alternatively, this file may be used under the terms of the GNU Lesser
    General Public License version 2.1 or version 3 as published by the Free
    Software Foundation and appearing in the file LICENSE.LGPLv21 and
    LICENSE.LGPLv3 included in the packaging of this file. Please review the
    following information to ensure the GNU Lesser General Public License
    requirements will be met: https://www.gnu.org/licenses/lgpl.html and
    http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
*/

#ifndef __JasonQt_TcpServer_h__
#define __JasonQt_TcpServer_h__

// Qt lib import
#include <QTcpServer>

// JasonQt lib import
#include "JasonQt_TcpSocket.h"

namespace JasonQt_Tcp
{

class ServerSettings
{
    PropertyDeclare(quint16, connectPort, setConnectPort, = 0)
    PropertyDeclare(int, connectThreadMaxCount, setConnectThreadMaxCount, = 5)
    PropertyDeclare(int, connectSingleThreadMaxCount, setConnectSingleThreadMaxCount, = 150)

    PropertyDeclare(Jde::Key, jdeKey, setJdeKey, = Jde::Key("PublicKey"))
    PropertyDeclare(bool, jdeUse, setJdeUse, = true)

public:
    ServerSettings(void) = default;

    inline ServerSettings(const std::function<void(ServerSettings &self)> &begin){ begin(*this); }
};

class ServerSocket: public Socket
{
private:
    qintptr m_socketDescriptor;

public:
    ServerSocket(qintptr socketDescriptor, const SocketSettings &socketSettings);

    bool readConnect(QTcpSocket *socket);
};

class ServerStart: public QObject
{
    Q_OBJECT

public:
    ServerStart(void);

    void newConnect(ServerSocket *socket);

private slots:
    void slotNewConnect(ServerSocket *socket);

signals:
    void signalNewConnect(ServerSocket *socket);
};

class Server: public QThread
{
    Q_OBJECT

private:
    ServerStart *m_start = NULL;

    QSet<Socket *> m_sockets;

public:
    inline QThread *socketThread(void) { return m_start->thread(); }

    inline int socketCount(void) const { return m_sockets.size(); }

    void newConnect(ServerSocket *socket);

public slots:
    void connected(Socket *socket);

    void disconnected(Socket *socket);

private:
    void run(void);
};

class ServerListen: public QTcpServer
{
    Q_OBJECT

private:
    void incomingConnection(qintptr socketDescriptor);

signals:
    void newConnect(qintptr socketDescriptor);
};

class ServerManage: public QThread
{
    Q_OBJECT

private:
    ServerSettings m_serverSettings;
    SocketSettings m_socketSettings;

    QVector<Server *> m_servers;

    PtrPropertyDeclare(std::function<bool(const QByteArray &accepted, QByteArray &send, Socket *socket)>, inspectionWithByteArray, setInspection, = NULL)
    PtrPropertyDeclare(std::function<void(Jdn::Set &accepted, Jdn::Set &send, Socket *socket)>, inspectionWithNodeControl, setInspection, = NULL)

    QHash<int32_t, std::function<void(Jdn::Set &accepted, Jdn::Set &send, Socket *socket)> > *m_inspectionWithNum = NULL;

    quint32 m_startTime = QDateTime::currentDateTime().toTime_t();

public:
    ServerManage(const ServerSettings &serverSettings, const SocketSettings &socketSettings);

    ~ServerManage(void);

    int socketCount(void);

    void setInspection(const int32_t &num, const std::function<void(Jdn::Set &accepted, Jdn::Set &send, Socket *socket)> &inspection);

    quint32 howLongBeforeStart(void) const;

    static QString connectTimeToString(const int &connectTime);

private:
    void run(void);

private slots:
    void newConnect(const qintptr &socketDescriptor);

    void accepted(Socket *socket, const QByteArray &data);

    void slotConnected(Socket *socket);

    void slotDisconnected(Socket *socket, const SocketExitFlag &exitFlag, const int &connectTime);

signals:
    void connected(JasonQt_Tcp::Socket *socket);

    void disconnected(JasonQt_Tcp::Socket *socket, const SocketExitFlag exitFlag, const int connectTime);
};

}

#endif//__JasonQt_TcpServer_h__
