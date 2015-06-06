#include "JasonQt_Settings.h"

using namespace JasonQt_Settings;

QString JasonQt_Settings::documentsPath(const QString &projectName, const QString &projectGroupName)
{
    assert(!projectName.isEmpty());
    assert(!projectGroupName.isEmpty());

#if (defined Q_OS_IOS)

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    dir.cdUp();
    dir.cd("Documents");
    return dir.path() + "/";

#elif (defined Q_OS_ANDROID)

    return "/sdcard/" + projectGroupName + "." + projectName + "/";

#elif (defined Q_OS_MAC)

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    dir.cdUp();
    dir.cd("C");
    return dir.path() + "/" + projectGroupName + "." + projectName + "/";

#elif (defined Q_OS_WIN)

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    dir.cdUp();
    return dir.path() + "/" + projectGroupName + "." + projectName + "/";

#else

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    return dir.path() + "/" + projectGroupName + "." + projectName + "/";

#endif
}

QSharedPointer<QSettings> JasonQt_Settings::settingsFile(const QString &fileName, const QString &projectName, const QString &projectGroupName)
{
    assert(!fileName.isEmpty());

    return QSharedPointer<QSettings>(new QSettings(JasonQt_Settings::documentsPath(projectName, projectGroupName) + fileName, QSettings::IniFormat));
}

Set::Set(const QString &fileName, const QString &groupName, const QString &projectName, const QString &projectGroupName):
    m_fileName(fileName),
    m_groupName(groupName),
    m_projectName(projectName),
    m_projectGroupName(projectGroupName)
{
    this->read();
}

Set::~Set(void)
{
    if(m_timer)
    {
        if(m_timer->isActive())
        {
            this->save();
        }
        delete m_timer;
    }
}

QJsonValue Set::operator[](const QString &key)
{
    if(!m_data.contains(key))
    {
        this->readySave();
    }
    return m_data[key];
}

QString Set::filePath(void) const
{
    return JasonQt_Settings::documentsPath(m_projectName, m_projectGroupName) + m_fileName;
}

bool Set::contains(const QString &key)
{
    return m_data.contains(key);
}

QJsonValue Set::value(const QString &key, const QJsonValue &defaultValue)
{
    if(!m_data.contains(key))
    {
        this->setValue(key, defaultValue);
    }
    return m_data[key];
}

QJsonValue Set::value(const QString &key)
{
    if(!m_data.contains(key))
    {
        return QJsonValue();
    }
    return m_data[key];
}

void Set::setValue(const QString &key, const QJsonValue &data)
{
    m_data[key] = data;
    this->readySave();
}

void Set::save(void)
{
    auto settings(JasonQt_Settings::settingsFile(m_fileName, m_projectName, m_projectGroupName));

    settings->beginGroup(m_groupName);

    for(auto it = m_data.begin(); it != m_data.end(); it++)
    {
        QJsonObject buf;
        buf["type"] = (int)it.value().type();
        buf["data"] = it.value();
        settings->setValue(it.key(), QJsonDocument(buf).toJson());
    }

    settings->endGroup();
}

void Set::readySave(const int &wait)
{
    if(!m_timer)
    {
        m_timer = new QTimer;
        m_timer->setSingleShot(true);

        connect(m_timer, SIGNAL(timeout()), this, SLOT(save()));
    }

    if(m_timer->isActive())
    {
        m_timer->stop();
    }

    m_timer->start(wait);
}

void Set::read(void)
{
    auto settings(JasonQt_Settings::settingsFile(m_fileName, m_projectName, m_projectGroupName));

    settings->beginGroup(m_groupName);

    m_data.clear();
    for(const auto &now: settings->allKeys())
    {
        const auto &&data = QJsonDocument::fromJson(settings->value(now).toByteArray()).object();
        switch(data["type"].toInt())
        {
            case QJsonValue::Bool:
            {
                m_data[now] = data["data"].toBool();
                break;
            }
            case QJsonValue::Double:
            {
                m_data[now] = data["data"].toDouble();
                break;
            }
            case QJsonValue::String:
            {
                m_data[now] = data["data"].toString();
                break;
            }
            default:
            {
                qDebug() << "JasonQt_Settings::Set::read: Not support type:" << data["type"].toInt();
                break;
            }
        }
    }

    settings->endGroup();
}
