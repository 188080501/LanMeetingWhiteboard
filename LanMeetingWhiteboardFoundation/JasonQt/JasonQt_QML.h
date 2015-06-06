/*
    JasonQt_QML

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

#ifndef __JasonQt_QML_h__
#define __JasonQt_QML_h__

// Qt lib import
#include <QtQml>

// JasonQt lib import
#include "JasonQt_Foundation.h"
#include "JasonQt_Net.h"

#ifdef QT_NETWORK_LIB
#   include <QHostInfo>
#endif

#ifdef QT_SENSORS_LIB
#   include <QAccelerometer>
#endif

namespace JasonQt_QML
{

class Tool: public QObject
{
    Q_OBJECT

public slots:
    double calculateDoubleScale(const double &currentA, const double &currentB, const double &sourceA, const double &sourceB, const double &maxScale = 4.0, const double &minScale = 0.5);

    double calculateSingleScale(const double &current, const double &source, const double &maxScale = 4.0, const double &minScale = 0.5);

    QString stringRightJustified(const QString &string, const int &width, const QString &fill = QString(" "));

    QString stringLeftJustified(const QString &string, const int &width, const QString &fill = QString(" "));

    QString stringToHash(const QString &string);

    QString deviceName(void);

    bool isDesktop(void);

    void delay(const int &time);

    void delayWithEventLoop(const int &time);
};

#ifdef QT_SENSORS_LIB
class Pseudo3DSensors: public QObject
{
    Q_OBJECT

private:
    QAccelerometer m_accelerometer;
    QTimer m_timer;

public:
    Pseudo3DSensors(void);

public slots:
    void start(const int &interval = 500);

    void stop(void);

private slots:
    void timeout(void);

signals:
    void indexChange(const double a, const double b, const double c);
};

#endif

}

#endif//__JasonQt_QML_h__
