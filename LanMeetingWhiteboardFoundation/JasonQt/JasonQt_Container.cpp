#include "JasonQt_Container.h"

QString JasonQt_HashChar::toString(void) const
{
    return QByteArray(m_data, 40);
}

void JasonQt_HashChar::set(const QByteArray &target)
{
    this->setSource(JasonQt_HashString(target));
}

void JasonQt_HashChar::set(const QString &target)
{
    this->set(target.toLatin1());
}

void JasonQt_HashChar::setSource(const QString &target)
{
    const auto &&TargetSize = target.size();
    const auto &&TargetString = target.toLatin1();

    if(TargetSize < 41)
    {
        memset(m_data, '\0', 40);
        memcpy(m_data, TargetString.data(), TargetSize);
    }
    else
    {
        memcpy(m_data, TargetString.data(), 40);
    }
}

JasonQt_HashChar::operator QString(void) const
{
    return this->toString();
}

JasonQt_HashChar::operator QVariant(void) const
{
    return this->toString();
}

JasonQt_HashChar::operator QJsonValue(void) const
{
    return this->toString();
}

JasonQt_HashChar &JasonQt_HashChar::operator=(const QByteArray &other)
{
    this->set(other);
    return *this;
}

JasonQt_HashChar &JasonQt_HashChar::operator=(const QString &other)
{
    this->set(other);
    return *this;
}

JasonQt_HashChar &JasonQt_HashChar::operator=(const char *other)
{
    this->set(QString(other));
    return *this;
}

bool JasonQt_HashChar::operator==(const QString &other) const
{
    return this->toString() == other;
}

bool JasonQt_HashChar::operator!=(const QString &other) const
{
    return this->toString() != other;
}

bool JasonQt_HashChar::operator<(const JasonQt_HashChar &other) const
{
    return this->toString() < other.toString();
}

// JasonQt_RefreshControl
void JasonQt_RefreshControl::setInterval(const int &interval)
{
    m_interval = interval;
    if(m_autoRefresh)
    {
        this->setAutoRefresh(true);
    }
}

void JasonQt_RefreshControl::setAutoRefresh(const bool &autoRefresh)
{
    if(!m_autoRefreshTimer)
    {
        m_autoRefreshTimer = new QTimer;
        connect(m_autoRefreshTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    }
    
    m_autoRefresh = autoRefresh;
    if(m_autoRefresh)
    {
        if(m_autoRefreshTimer->isActive())
        {
            m_autoRefreshTimer->stop();
        }
        m_autoRefreshTimer->start(m_interval);
    }
    else
    {
        m_autoRefreshTimer->stop();
    }
}

void JasonQt_RefreshControl::setRefreshOnThread(const bool &refreshOnThread)
{
    m_refreshOnThread = refreshOnThread;
}

void JasonQt_RefreshControl::startRefresh(const std::function<void ()> &fun, const int &interval, const bool &refreshOnStart)
{
    auto timer = new QTimer;
    connect(timer, &QTimer::timeout, fun);
    timer->start(interval);

    if(refreshOnStart)
    {
        fun();
    }
}

void JasonQt_RefreshControl::timeout(void)
{
    if(m_refreshOnThread)
    {
#ifndef QT_CONCURRENT_LIB
        qDebug("JasonQt_RefreshControl: error1");
#else
        QtConcurrent::run([=](){ this->refresh("AutoRefresh"); });
#endif
    }
    else
    {
        this->refresh("AutoRefresh");
    }
}

// JasonQt_AvailableSocketManageAbstract
JasonQt_AvailableSocketManageAbstract::Node::Node(JasonQt_Tcp::Socket *socket_, const qint32 &socketFlag_, const quint32 &lastConnectTime_):
    socket(socket_),
    socketFlag(socketFlag_),
    lastConnectTime(lastConnectTime_)
{ }

void JasonQt_AvailableSocketManageAbstract::setServerManage(JasonQt_Tcp::ServerManage *serverManage)
{
    QObject::connect(serverManage, &JasonQt_Tcp::ServerManage::connected, [&](JasonQt_Tcp::Socket *socket)
    {
        m_mutex.lock();

        m_availableSocket.insert(socket, m_currentSocketFlag++);

        m_mutex.unlock();
    });
    QObject::connect(serverManage, &JasonQt_Tcp::ServerManage::disconnected, [&](JasonQt_Tcp::Socket *socket)
    {
        m_mutex.lock();

        auto &&it = m_availableSocket.find(socket);
        if(it != m_availableSocket.end())
        {
            m_availableSocket.erase(it);
        }
        else
        {
            qDebug("JasonQt_AvailableSocketManageAbstract: error1");
        }

        m_mutex.unlock();
    });
}

void JasonQt_AvailableSocketManageAbstract::setMaxInterval(const quint32 &maxInterval)
{
    m_maxInterval = maxInterval;
}

// JasonQt_Concurrent
#ifdef QT_CONCURRENT_LIB
JasonQt_Concurrent::JasonQt_Concurrent(const QList<QList<std::function<std::pair<bool, QString> ()> > > &waitRun, const int &minThreadCount):
    m_waitRun(waitRun),
    m_minThreadCount(minThreadCount)
{ }

std::pair<bool, QString> JasonQt_Concurrent::run(void)
{
    std::pair<bool, QString> needReturn({ true, { } });

#if QT_VERSION >= 0x050400
    int befor = -1;

    if(QThreadPool::globalInstance()->maxThreadCount() < m_minThreadCount)
    {
        befor = QThreadPool::globalInstance()->maxThreadCount();
        QThreadPool::globalInstance()->setMaxThreadCount(m_minThreadCount);
    }

    for(const auto &runs: m_waitRun)
    {
        QEventLoop eventLoop;
        QList< QFutureWatcher< std::pair<bool, QString> > *> watchers;

        QMutex mutex;
        int flag = runs.size();

        QTimer::singleShot(0, [&]()
        {
            for(const auto &run: runs)
            {
                auto buf = new QFutureWatcher< std::pair<bool, QString> >;
                QObject::connect(buf, &QFutureWatcher< std::pair<bool, QString> >::finished, [&]()
                {
                    mutex.lock();

                    if(!(--flag))
                    {
                        eventLoop.quit();
                    }

                    mutex.unlock();
                });

                buf->setFuture(QtConcurrent::run(run));

                watchers.push_back(buf);
            }
        });

        eventLoop.exec();

        bool flag2 = false;

        for(const auto &now: watchers)
        {
            const auto &&buf = now->result();
            if(!buf.first)
            {
                flag2 = true;
                needReturn = buf;
            }
        }

        qDeleteAll(watchers);

        if(flag2)
        {
            break;
        }
    }

    if(befor != -1)
    {
        QThreadPool::globalInstance()->setMaxThreadCount(befor);
    }

#endif

    return needReturn;
}
#endif

// JasonQt_TimeDataPackage
JasonQt_TimeDataPackage::JasonQt_TimeDataPackage(void):
    time(0),
    interval(0)
{ }

JasonQt_TimeDataPackage::JasonQt_TimeDataPackage(const quint32 &time_, const quint32 &interval_):
    time(time_),
    interval(interval_)
{ }
