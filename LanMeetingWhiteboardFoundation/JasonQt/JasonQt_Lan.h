/*
    JasonQt_Lan

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

#ifndef __JasonQt_Lan_h__
#define __JasonQt_Lan_h__

// Qt lib import
#include <QMap>

#include <QTcpSocket>
#include <QTcpServer>
#include <QUdpSocket>

#include <QtConcurrent>

// Jason lib import
#include "Jdn.h"

// JasonQt lib import
#include "JasonQt_Foundation.h"
#include "JasonQt_Net.h"

namespace JasonQt_Lan
{

class LanSearcher: public QObject
{
    Q_OBJECT

private:
    quint16 m_port;
    int m_triggeredInterval;
    int m_timeout;

    QTimer m_timer;

    QNetworkAddressEntry m_currentAddressEntry;
    QMap<quint32, qint64> m_availableIp;

    QUdpSocket m_socket;

    Jde::Key m_jdeKey;

    PtrPropertyDeclare(std::function<void(Jdn::Set &appendData)>, onSendCallback, setOnSendCallback, = NULL)
    PtrPropertyDeclare(std::function<void(Jdn::Set &data)>, onAcceptedCallback, setOnAcceptedCallback, = NULL)

public:
    LanSearcher(const quint16 &port, const int &triggeredInterval = 1000, const int timeout = 5 * 1000, const QString &jdeKey = "JasonQt_LanSearcher");

    void foreachAvailableIP(const std::function<void (const QHostAddress &address)> &foreachIP);

    inline bool ipIsContains(const quint32 &ip) const { return m_availableIp.contains(ip); }

    inline QNetworkAddressEntry currentAddressEntry(void) const { return m_currentAddressEntry; }

public slots:
    bool begin(const bool &printMessage = false);

    void ping(void);

private slots:
    void onTriggered(void);

    void onAccepted(void);

signals:
    void newConnect(const QHostAddress address);

    void disConnect(const QHostAddress address);
};

class TcpListener: public QTcpServer
{
    Q_OBJECT

public:
    void incomingConnection(qintptr socketDescriptor)
    {
        emit newConnect(socketDescriptor);
    }

signals:
    void newConnect(qintptr socketDescriptor);
};

class LanSocket: public QObject
{
    Q_OBJECT

private:
    LanSearcher *m_lanSearcher;
    quint16 m_port;

    TcpListener m_listener;

    QThreadPool m_sendThreadPool;
    QThreadPool m_acceptedThreadPool;

    PtrPropertyDeclare(std::function<void(Jdn::Set &accepted, Jdn::Set &send)>, onAcceptedCallback, setOnAcceptedCallback, = NULL)

public:
    LanSocket(LanSearcher *lanSearcher, const quint16 &port, const int &maxThreadCount = 2);

public slots:
    bool begin(void);

    void send(Jdn::Set &data);

    void send(Jdn::Set &data, const QList<QHostAddress> &address);

private:
    void send(const QHostAddress &address, const QByteArray &data);

    void accepted(QTcpSocket &socket, const bool &needReply);
};

}

#endif//__JasonQt_Lan_h__
