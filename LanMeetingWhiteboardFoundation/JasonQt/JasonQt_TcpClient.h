/*
    JasonQt_TcpClient

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

#ifndef __JasonQt_TcpClient_h__
#define __JasonQt_TcpClient_h__

// JasonQt lib import
#include "JasonQt_TcpSocket.h"

namespace JasonQt_Tcp
{

class ClientSettings
{
    PropertyDeclare(bool, workOnSingleThread, setWorkOnSingleThread, = true)

    PropertyDeclare(QString, connectAddress, setConnectAddress, = "127.0.0.1")
    PropertyDeclare(quint16, connectPort, setConnectPort, = 0)
    PropertyDeclare(int,     connectTryMax, setConnectTryMax, = 3)
    PropertyDeclare(int,     connectTrtDelay, setConnectTrtDelay, = 3 * 1000)
    PropertyDeclare(bool,    connectHoldConnect, setConnectHoldConnect, = true)
    PropertyDeclare(int,     connectCount, setConnectCount, = 30)

    PropertyDeclare(Jde::Key, jdeKey, setJdeKey, = Jde::Key("PublicKey"))
    PropertyDeclare(bool, jdeUse, setJdeUse, = true)

public:
    ClientSettings(void) = default;

    ClientSettings(const std::function<void(ClientSettings &self)> &begin){ begin(*this); }
};

class ClientSocket: public Socket
{
private:
    QString m_connectAddress;
    quint16 m_connectPort;

public:
    ClientSocket(const QString &connectAddress, const quint16 &connectPort, const SocketSettings &socketSettings);

    bool readConnect(QTcpSocket *socket);
};

class ClientStart: public QObject
{
    Q_OBJECT

public:
    class ReadySend
    {
    public:
        virtual void readySend(const QByteArray &data, const quint32 &flag) = 0;
    };

private:
    ReadySend *m_readySend;

public:
    ClientStart(ReadySend *readySend);

    void send(const QByteArray &data, const quint32 &flag);

private slots:
    void slotsSend(const QByteArray &data, const quint32 &flag);

signals:
    void signalsSend(const QByteArray &data, const quint32 flag);
};

class ClientManage: public QThread, public ClientStart::ReadySend
{
    Q_OBJECT

private:
    ClientSettings m_clientSettings;
    SocketSettings m_socketSettings;

    QMap<Socket *, quint32> m_sockets;
    ClientStart *m_start = NULL;

    QMutex m_mutex;
    quint32 m_currentRunFlag = 1;

    bool m_isRun = false;

    PtrPropertyDeclare(std::function<bool(const QByteArray &accepted, Socket *socket)>, inspectionexceptionWithByteArray, setInspectionexceptionWithByteArray, = NULL)
    PtrPropertyDeclare(std::function<void(Jdn::Set &accepted, Socket *socket)>, inspectionExceptionWithNodeControl, setInspectionExceptionWithNodeControl, = NULL)

public:
    static bool m_exitByApplication;

public:
    ClientManage(const ClientSettings &clientSettings, const SocketSettings &socketSettings);

    ~ClientManage(void);

    int socketCount(void) const { return m_sockets.size(); }

public slots:
    void send(const QByteArray &send);

    bool send(const QByteArray &send, QByteArray &accepted);

    void send(Jdn::Set &send);

    bool send(Jdn::Set &send, Jdn::Set &accepted);

    void closeAllConnect(void);

private:
    void readySend(const QByteArray &data, const quint32 &flag);

private slots:
    void disconnected(Socket *socket);

    void accepted(Socket *socket, const QByteArray &data);

private:
    void run(void);

    quint32 getRunFlag(void);

    void waitForRun(void);

private: signals:
    void finished(const quint32 flag, const QByteArray data, const bool done);
};

// Function define
QString standardHandleProcess(ClientManage &client,
                              Jdn::Set &send,
                              Jdn::Set &accepted,
                              const int32_t &OKFlag,
                              const int32_t &FailFlag,
                              const bool &inspectionFirstNode = true);

QString standardHandleProcess(ClientManage &client, Jdn::Set &send,
                              const int32_t &OKFlag, const int32_t &FailFlag);

QString standardHandleProcess(ClientManage &client, const Jdn::Node &node,
                              const int32_t &OKFlag, const int32_t &FailFlag);

template<typename PackageType>
QString standardHandleProcess(ClientManage &client, const PackageType &package,
                              const int32_t &OKFlag, const int32_t &FailFlag);

template<typename PackageType>
QString standardHandleProcess(ClientManage &client, const PackageType &package, Jdn::Set &accepted,
                              const int32_t &OKFlag, const int32_t &FailFlag);

template<typename PackageType, typename TargetType>
QString standardHandleProcess(ClientManage &client, const PackageType &package, TargetType &target,
                              const int32_t &OKFlag, const int32_t &FailFlag);

template<typename PackageType, typename TargetType>
QString standardHandleProcess(ClientManage &client, const PackageType &package, QVector<TargetType> &target,
                              const int32_t &OKFlag, const int32_t &FailFlag);

template<typename PackageType, typename TargetType>
QString standardHandleProcess(ClientManage &client, const QVector<PackageType> &packages, QVector<TargetType> &target,
                              const int32_t &OKFlag, const int32_t &FailFlag,
                              const std::function<bool(const Jdn::Node &)> &doubleCheck);

template<typename PackageType, typename TargetType>
QString standardHandleProcess(ClientManage &client, const QVector<PackageType> &packages, QVector<TargetType> &target,
                              const int32_t &OKFlag, const int32_t &FailFlag);

}

template<typename PackageType>
QString JasonQt_Tcp::standardHandleProcess(ClientManage &client, const PackageType &package,
                                           const int32_t &OKFlag, const int32_t &FailFlag)
{
    Jdn::Set send;
    send.push_back_single_data(package);
    return standardHandleProcess(client, send, OKFlag, FailFlag);
}

template<typename PackageType>
QString JasonQt_Tcp::standardHandleProcess(ClientManage &client, const PackageType &package, Jdn::Set &accepted,
                                           const int32_t &OKFlag, const int32_t &FailFlag)
{
    Jdn::Set send;
    send.push_back_single_data(package);
    return standardHandleProcess(client, send, accepted, OKFlag, FailFlag);
}

template<typename PackageType, typename TargetType>
QString JasonQt_Tcp::standardHandleProcess(ClientManage &client, const PackageType &package, TargetType &target,
                                           const int32_t &OKFlag, const int32_t &FailFlag)
{
    Jdn::Set accepted;

    const auto &&flag = standardHandleProcess(client, package, accepted, OKFlag, FailFlag);
    if(flag != "OK")
    {
        return flag;
    }
    accepted.copy_current_from_single_data(target);
    return flag;
}

template<typename PackageType, typename TargetType>
QString JasonQt_Tcp::standardHandleProcess(ClientManage &client, const PackageType &package, QVector<TargetType> &target,
                                           const int32_t &OKFlag, const int32_t &FailFlag)
{
    Jdn::Set accepted;
    TargetType buf;
    QVector<TargetType> buf2;

    const auto &&flag = standardHandleProcess(client, package, accepted, OKFlag, FailFlag);
    if(flag != "OK")
    {
        return flag;
    }

    while(accepted.next())
    {
        if((accepted.currentNodeNum() != buf.packageNum()) || (accepted.currentNodeType() != Jdn::WithByteArrayNode))
        {
            if(accepted.currentNodeType() != Jdn::WithStringNode)
            {
                return "DifferentNodeType(2)";
            }
            if(accepted.currentNodeNum() != FailFlag)
            {
                return "DifferentFailFlag(2)";
            }
            return accepted.currentNodeQString();
        }
        if(!accepted.copy_current_from_single_data(buf))
        {
            return "CopyToNodeError";
        }
        else
        {
            buf2.push_back(buf);
        }
    }

    target += buf2;
    return flag;
}

template<typename PackageType, typename TargetType>
QString JasonQt_Tcp::standardHandleProcess(ClientManage &client, const QVector<PackageType> &packages, QVector<TargetType> &target,
                                           const int32_t &OKFlag, const int32_t &FailFlag,
                                           const std::function<bool(const Jdn::Node &)> &doubleCheck)
{
    Jdn::Set send, accepted;
    TargetType buf;
    QVector<TargetType> buf2;

    for(const auto &now: packages)
    {
        send.push_back_single_data(now);
    }

    const auto &&flag = standardHandleProcess(client, send, accepted, OKFlag, FailFlag, false);
    if(flag != "OK")
    {
        return flag;
    }

    while(accepted.next())
    {
        if(accepted.currentNodeNum() == OKFlag)
        {
            continue;
        }
        if((accepted.currentNodeNum() != buf.packageNum()) || (accepted.currentNodeType() != Jdn::WithByteArrayNode))
        {
            if(doubleCheck(*accepted.currentNode()))
            {
                continue;
            }
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
        if(!accepted.copy_current_from_single_data(buf))
        {
            if(doubleCheck(*accepted.currentNode()))
            {
                continue;
            }
            return "CopyToNodeError";
        }
        else
        {
            buf2.push_back(buf);
        }
    }

    target += buf2;
    return flag;
}

template<typename PackageType, typename TargetType>
QString JasonQt_Tcp::standardHandleProcess(ClientManage &client, const QVector<PackageType> &packages, QVector<TargetType> &target,
                                           const int32_t &OKFlag, const int32_t &FailFlag)
{
    return standardHandleProcess(client, packages, target, OKFlag, FailFlag, [](const Jdn::Node &)->bool{ return false; });
}

#endif//__JasonQt_TcpClient_h__
