/*
    JasonQt_Vop

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

#ifndef __JasonQt_Vop_h__
#define __JasonQt_Vop_h__

#ifndef QT_CONCURRENT_LIB
#   error("Plwaer add concurrent in pro file")
#endif

#ifndef QT_MULTIMEDIA_LIB
#   error("Plwaer add multimedia in pro file")
#endif

// Qt lib import
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QBuffer>

#include <QSound>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QAudioOutput>

#include <QtConcurrent>

// JasonQt lib import
#include "JasonQt_Net.h"

namespace JasonQt_Vop
{

class BaiduTokenFoundation: public QObject
{
    Q_OBJECT

protected:
    JasonQt_Net::HTTP m_http;

    QString m_apiKey;
    QString m_secretKey;
    QString m_token;

public:
    BaiduTokenFoundation(const QString &apiKey, const QString &secretKey);

public slots:
    bool refreshToken(const bool &onNewThread = false);
};

class BaiduVop: public BaiduTokenFoundation
{
    Q_OBJECT

private:
    QAudioInput *m_audioInput = NULL;
    QByteArray m_buf;
    QBuffer *m_buffer = NULL;

public:
    BaiduVop(const QString &apiKey, const QString &secretKey);

    ~BaiduVop(void);

    void setDevice(const QAudioDeviceInfo &device);

public slots:
    bool start(void);

    std::pair<bool, QString> finish(void);
};

class BaiduText2Audio: public BaiduTokenFoundation
{
    Q_OBJECT

private:
    QAudioOutput *m_audioOutput = NULL;
    QBuffer *m_buffer = NULL;

public:
    BaiduText2Audio(const QString &apiKey, const QString &secretKey);

    ~BaiduText2Audio(void);

    void setDevice(const QAudioDeviceInfo &device);

public slots:
    bool play(const QString &message);
};

}

#endif//__JasonQt_Vop_h__
