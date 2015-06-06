/*
    JasonQt_Net

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

#ifndef __JasonQt_Net_h__
#define __JasonQt_Net_h__

#ifndef QT_NETWORK_LIB
#   error("Plwaer add network in pro file")
#endif

// Qt lib import
#include <QUrl>
#include <QString>
#include <QByteArray>
#include <QEventLoop>
#include <QTimer>

#include <QHostInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>

namespace JasonQt_Net
{

QNetworkAddressEntry getNetworkAddressEntry(void);

QString getDeviceName(void);

class HTTP: public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager m_manage;
    QNetworkReply *m_reply = NULL;
    QTimer m_timer;

public:
    HTTP(void);

public slots:
    void get(const QUrl &url, const int &maxTime = 30000);

    void post(const QNetworkRequest &networkRequest, const QByteArray &append, const int &maxTime = 30000);

    void stop(void);

    bool get(const QUrl &url, QByteArray &target, const int &maxTime = 30000);

    bool post(const QNetworkRequest &networkRequest, const QByteArray &append, QByteArray &target, const int &maxTime = 30000);

private slots:
    void onFinished(QNetworkReply *reply);

    void onTimeout(void);

    void onError(const QNetworkReply::NetworkError &error);

signals:
    void finished(const QByteArray data);

    void error(const QNetworkReply::NetworkError error);
};

}

#endif//__JasonQt_Net_h__
