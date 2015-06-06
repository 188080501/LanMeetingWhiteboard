/*
    JasonQt_TcpSocket

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

#ifndef __JasonQt_TcpSocket_h__
#define __JasonQt_TcpSocket_h__

#ifndef QT_NETWORK_LIB
#   error("Plwaer add network in pro file")
#endif

// Qt lib import
#include <QTcpSocket>

// Jdn lib import
#include "Jdn.h"

// JasonQt lib import
#include "JasonQt_Foundation.h"

namespace JasonQt_Tcp
{

class SocketSettings
{
    PropertyDeclare(int, connectWaitTime,    setConnectWaitTime,    = 15 * 1000)
    PropertyDeclare(int, connectMaxTime,     setConnectMaxTime,     = 3600 * 1000)
    PropertyDeclare(int, connectMaxReadTime, setConnectMaxReadTime, = 30 * 1000)

    PropertyDeclare(int,        dataMaxBufferSize,    setDataMaxBufferSize,    = 128 * 1024 * 1024)
    PropertyDeclare(int,        dataSendMaxSize,      setDataSendMaxSize,      = 8 * 1024 *1024)
    PropertyDeclare(QByteArray, dataSendStartFlag,    setDataSendStartFlag,    = QByteArray())
    PropertyDeclare(QByteArray, dataSendEndFlag,      setDataSendEndFlag,      = QByteArray())

    PropertyDeclare(bool,       dataUseAcceptedCheck, setDataUseAcceptedCheck, = true)
    PropertyDeclare(std::function<int(const QByteArray &, const QByteArray &)>, dataAcceptedCheck, setDataAcceptedCheck, = [](const QByteArray &buf, const QByteArray &)->int
    {
        /*
         * Return flag:
         *  -3: Error (Socket will be broken)
         *  -2: Invalid (Continue wait)
         *  -1: Just fine
         * >=0: Need tailor to (>=0)
         */

        switch(Jdn::Set::checkStreamSize(buf))
        {
            case Jdn::StreamSizeJustFine:       { return -1; }
            case Jdn::StreamSizeNeedWaitMore:   { return -2; }
            case Jdn::StreamSizeOverTargetSize: { return Jdn::Set::checkStreamTargetSize(buf) + 4; }
            case Jdn::StreamSizeOverMaxSize:    { return -3; }
        }

        return -3;
    })

public:
    SocketSettings(void) = default;

    SocketSettings(const std::function<void(SocketSettings &self)> &begin){ begin(*this); }
};

enum SocketExitFlag
{
    ExitFlag_ActiveExit = 1,
    ExitFlag_ConnectMaxTimeout,
    ExitFlag_ConnectMaxReadTimeout,
    ExitFlag_AcceptedBufferError,
    ExitFlag_SocketDisconnected,
    ExitFlag_SocketError
};

class Socket: public QObject
{
    Q_OBJECT

protected:
    QTcpSocket *m_socket = NULL;
    const SocketSettings &m_socketSettings;

    quint32 m_connectSartTime = 0;
    int m_needToWritten = 0;
    QList<QByteArray> *m_sendBuffer = NULL;
    QByteArray m_acceptedBuffer;

    QTimer *m_connectMaxTimeTimer = NULL;
    QTimer *m_connectMaxReadTimeTimer = NULL;

public:
    Socket(const SocketSettings &socketSettings);

    ~Socket(void);

    inline const QTcpSocket *socket(void) const { return m_socket; }

public slots:
    bool begin(void);

    void close(void);

    void send(const QByteArray &data);

protected:
    virtual bool readConnect(QTcpSocket *socket) = 0;

private slots:
    void readyRead(void);

    void bytesWritten(const qint64 &bytes);

    void connectMaxTimeout(void);

    void connectMaxReadTimeout(void);

    void socketDisconnected(void);

    void socketError(const QAbstractSocket::SocketError &error);

    void readyClose(void);

    void readySend(const QByteArray &data);

signals:
    void conneced(Socket *socket);

    void disconnected(Socket *socket, const SocketExitFlag exitFlag, int connectTime);

    void accepted(Socket *socket, const QByteArray data);

private: signals:
    void needToSend(const QByteArray data);
};

}

#endif//__JasonQt_TcpSocket_h__
