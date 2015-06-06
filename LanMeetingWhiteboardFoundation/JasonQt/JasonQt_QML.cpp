#include "JasonQt_QML.h"
#include <QDebug>

// Tool
double JasonQt_QML::Tool::calculateDoubleScale(const double &currentA, const double &currentB, const double &sourceA, const double &sourceB, const double &maxScale, const double &minScale)
{
    const auto &&aFlag = currentA / sourceA;
    const auto &&bFlag = currentB / sourceB;
    const auto &flag = (aFlag < bFlag) ? (aFlag) : (bFlag);

    if(flag > maxScale)
    {
        return maxScale;
    }
    if(flag < minScale)
    {
        return minScale;
    }

    return ((0.99 < flag) && (flag < 1.01)) ? (1.0) : (flag);
}

double JasonQt_QML::Tool::calculateSingleScale(const double &current, const double &source, const double &maxScale, const double &minScale)
{
    const auto &&flag = current / source;

    if(flag > maxScale)
    {
        return maxScale;
    }
    if(flag < minScale)
    {
        return minScale;
    }

    return ((0.99 < flag) && (flag < 1.01)) ? (1.0) : (flag);
}

QString JasonQt_QML::Tool::stringRightJustified(const QString &string, const int &width, const QString &fill)
{
    return string.rightJustified(width, fill.at(0));
}

QString JasonQt_QML::Tool::stringLeftJustified(const QString &string, const int &width, const QString &fill)
{
    return string.leftJustified(width, fill.at(0));
}

QString JasonQt_QML::Tool::stringToHash(const QString &string)
{
    return JasonQt_HashString(string);
}

QString JasonQt_QML::Tool::deviceName(void)
{
    return JasonQt_Net::getDeviceName();
}

bool JasonQt_QML::Tool::isDesktop(void)
{
#if (defined Q_OS_MAC && !(defined Q_OS_IOS)) || (defined Q_OS_WIN) || (defined Q_OS_LINUX)
    return true;
#endif
    return false;
}

void JasonQt_QML::Tool::delay(const int &time)
{
    QThread::msleep(time);
}

void JasonQt_QML::Tool::delayWithEventLoop(const int &time)
{
    JasonQt_EventLoopSleep(time);
}

// Pseudo3DSensors
#ifdef QT_SENSORS_LIB
JasonQt_QML::Pseudo3DSensors::Pseudo3DSensors(void)
{
    connect(&m_timer, SIGNAL(timeout()), this ,SLOT(timeout()));
}

void JasonQt_QML::Pseudo3DSensors::start(const int &interval)
{
    m_timer.start(interval);
    m_accelerometer.start();
}

void JasonQt_QML::Pseudo3DSensors::stop(void)
{
    m_timer.stop();
    m_accelerometer.stop();
}

void JasonQt_QML::Pseudo3DSensors::timeout(void)
{
    const auto accelerometer = m_accelerometer.reading();
    if(accelerometer)
    {
        auto &&flag = accelerometer->x() / 10.0 / 2.0 + 0.5;
        if(flag > 1.0)
        {
            flag = 1.0;
        }
        else if(flag < 0.0)
        {
            flag = 0.0;
        }
        emit indexChange(flag, flag, flag);
    }
    else
    {
        emit indexChange(0.5, 0.5, 0.5);
    }
}
#endif
