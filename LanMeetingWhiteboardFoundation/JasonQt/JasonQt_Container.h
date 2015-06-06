/*
    JasonQt_Container

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

#ifndef __JasonQt_Container_h__
#define __JasonQt_Container_h__

// Qt lib import
#ifdef QT_CONCURRENT_LIB
#   include <QtConcurrent>
#   include <QFutureWatcher>
#endif

// JasonQt lib import
#include "JasonQt_Foundation.h"

#ifdef QT_NETWORK_LIB
#   include "JasonQt_TcpServer.h"
#endif

template<int stringSize, bool useUTF8>
class JasonQt_Char
{
private:
    char m_data[stringSize];

public:
    QString toString(void) const;

    void set(const QString &other);

    operator QString(void) const;

    operator QVariant(void) const;

    operator QJsonValue(void) const;

    JasonQt_Char &operator=(const QString &other);

    JasonQt_Char &operator=(const char *other);

    bool operator==(const QString &other) const;

    bool operator!=(const QString &other) const;

    bool operator<(const JasonQt_Char &other) const;
};

class JasonQt_HashChar
{
private:
    char m_data[40];

public:
    QString toString(void) const;

    void set(const QByteArray &target);

    void set(const QString &target);

    void setSource(const QString &target);

    operator QString(void) const;

    operator QVariant(void) const;

    operator QJsonValue(void) const;

    JasonQt_HashChar &operator=(const QByteArray &other);

    JasonQt_HashChar &operator=(const QString &other);

    JasonQt_HashChar &operator=(const char *other);

    bool operator==(const QString &other) const;

    bool operator!=(const QString &other) const;

    bool operator<(const JasonQt_HashChar &other) const;
};

class JasonQt_RefreshControl: public QObject
{
    Q_OBJECT

private:
    QTimer *m_autoRefreshTimer = NULL;
    int m_interval = 10000;
    bool m_autoRefresh = false;
    bool m_refreshOnThread = true;

public slots:
    void setInterval(const int &interval);

    void setAutoRefresh(const bool &autoRefresh);

    void setRefreshOnThread(const bool &refreshOnThread);

    virtual std::pair<bool, QString> refresh(const QString &flag = QString()) = 0;

    static void startRefresh(const std::function<void(void)> &fun, const int &interval = 10000, const bool &refreshOnStart = false);

private slots:
    void timeout(void);
};

class JasonQt_TimeDataPackage
{
public:
    quint32 time;
    quint32 interval;

public:
    JasonQt_TimeDataPackage(void);

    JasonQt_TimeDataPackage(const quint32 &time, const quint32 &interval);
};

template<typename KeyType, typename DataType>
class JasonQt_Map
{
protected:
    QVector<DataType> m_data;
    QHash<KeyType, const DataType *> m_dataFromKey;
    QMutex m_mutex;

public:
    inline operator QVector<DataType>(void);

    inline DataType operator[](const KeyType &key);

    inline JasonQt_Map &operator=(const QVector<DataType> &other);

    void insert(const DataType &newData, const bool &coverOld = true);

    void clear(void);

    int size(void);

    DataType find(const KeyType &key);

    const DataType &at(const int &index) const;

    QVector<DataType> data(void);

    void copyFrom(const QVector<DataType> &other);

    void foreachData(const std::function<bool(const DataType &data)> &each);

    template<typename FindKeyType>
    void foreachData(const std::function<bool(const DataType &data)> &each, const FindKeyType &findKey, const std::function<bool(const DataType &, const FindKeyType &)> &find);

private:
    void clear(int);

    void refreshKey(void);
};

#ifdef QT_CONCURRENT_LIB
class JasonQt_Concurrent
{
private:
    QList< QList< std::function< std::pair<bool, QString>(void) > > > m_waitRun;
    int m_minThreadCount;

public:
    JasonQt_Concurrent(const QList< QList< std::function< std::pair<bool, QString>(void) > > > &waitRun = QList< QList< std::function< std::pair<bool, QString>(void) > > >(), const int &minThreadCount = 2);

    std::pair<bool, QString> run(void);
};
#endif

#ifdef QT_NETWORK_LIB
class JasonQt_AvailableSocketManageAbstract
{
protected:
    class Node
    {
    public:
        JasonQt_Tcp::Socket *socket;
        qint32 socketFlag;
        quint32 lastConnectTime;

        Node(JasonQt_Tcp::Socket *socket, const qint32 &socketFlag, const quint32 &lastConnectTime);
    };

protected:
    QHash<JasonQt_Tcp::Socket *, qint32> m_availableSocket;
    QMutex m_mutex;
    quint32 m_maxInterval = 10;
    qint32 m_currentSocketFlag = 1;

public:
    void setServerManage(JasonQt_Tcp::ServerManage *serverManage);

    void setMaxInterval(const quint32 &maxInterval = 10);
};

template<typename KeyType>
class JasonQt_AvailableSingleSocketManage: public JasonQt_AvailableSocketManageAbstract
{
private:
    QHash<KeyType, Node> m_nodes;

public:
    JasonQt_AvailableSingleSocketManage(const int &maxIntervalSecond = 10);

    void refreshSocket(const KeyType &key, JasonQt_Tcp::Socket *socket);

    bool selectAvailable(const KeyType &key, const std::function<void(JasonQt_Tcp::Socket *)> &send);
};

template<typename KeyType>
class JasonQt_AvailableMultiSocketManage: public JasonQt_AvailableSocketManageAbstract
{
private:
    QHash<KeyType, QList<Node> > m_nodes;

public:
    JasonQt_AvailableMultiSocketManage(const int &maxIntervalSecond = 10);

    void refreshSocket(const KeyType &key, JasonQt_Tcp::Socket *socket);

    int selectAvailable(const KeyType &key, const std::function<void(JasonQt_Tcp::Socket *)> &send);
};
#endif

template<typename KeyType, typename DataType>
class JasonQt_CacheManage
{
private:
    typedef std::function<std::pair<bool, DataType>(const KeyType &key)> updateSourceType;
    PtrPropertyDeclare(updateSourceType, updateSource, setUpdateSource, = NULL)

    QHash<KeyType, DataType> m_data;
    QMutex m_mutex;

public:
    JasonQt_CacheManage(void) = default;

    JasonQt_CacheManage(const std::function<std::pair<bool, DataType>(const KeyType &key)> &updateSource);

    void update(const KeyType &key, const DataType &data);

    bool delete_(const KeyType &key);

    void clear(void);

    bool contains(const KeyType &key);

    bool select(const KeyType &key, DataType &target);

    std::pair<bool, DataType> select(const KeyType &key);
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~TemplateFunctionDefine~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// JasonQt_Char
template<int stringSize, bool useUTF8>
QString JasonQt_Char<stringSize, useUTF8>::toString(void) const
{
    if(useUTF8)
    {
        return QString::fromUtf8(QByteArray(m_data, (m_data[stringSize - 1]) ? (stringSize) : (strlen(m_data))));
    }
    else
    {
        return QByteArray(m_data, (m_data[stringSize - 1]) ? (stringSize) : (strlen(m_data)));
    }
}

template<int stringSize, bool useUTF8>
void JasonQt_Char<stringSize, useUTF8>::set(const QString &other)
{
    int TargetSize;
    QByteArray TargetString;

    if(useUTF8)
    {
        TargetSize = other.toUtf8().size();
        TargetString = other.toUtf8();
    }
    else
    {
        TargetSize = other.toLatin1().size();
        TargetString = other.toLatin1();
    }

    if(TargetSize < stringSize)
    {
        memset(m_data, '\0', stringSize);
        memcpy(m_data, TargetString.data(), TargetSize);
    }
    else
    {
        memcpy(m_data, TargetString.data(), stringSize);
    }
}

template<int stringSize, bool useUTF8>
JasonQt_Char<stringSize, useUTF8>::operator QString(void) const
{
    return this->toString();
}

template<int stringSize, bool useUTF8>
JasonQt_Char<stringSize, useUTF8>::operator QVariant(void) const
{
    return this->toString();
}

template<int stringSize, bool useUTF8>
JasonQt_Char<stringSize, useUTF8>::operator QJsonValue(void) const
{
    return this->toString();
}

template<int stringSize, bool useUTF8>
JasonQt_Char<stringSize, useUTF8> &JasonQt_Char<stringSize, useUTF8>::operator=(const QString &other)
{
    this->set(other);
    return *this;
}

template<int stringSize, bool useUTF8>
JasonQt_Char<stringSize, useUTF8> &JasonQt_Char<stringSize, useUTF8>::operator=(const char *other)
{
    this->set(QString(other));
    return *this;
}

template<int stringSize, bool useUTF8>
bool JasonQt_Char<stringSize, useUTF8>::operator==(const QString &other) const
{
    return this->toString() == other;
}

template<int stringSize, bool useUTF8>
bool JasonQt_Char<stringSize, useUTF8>::operator!=(const QString &other) const
{
    return this->toString() != other;
}

template<int stringSize, bool useUTF8>
bool JasonQt_Char<stringSize, useUTF8>::operator<(const JasonQt_Char &other) const
{
    return this->toString() < other.toString();
}

// JasonQt_Map
template<typename KeyType, typename DataType>
JasonQt_Map<KeyType, DataType>::operator QVector<DataType>(void)
{
    m_mutex.lock();

    auto buf = m_data;

    m_mutex.unlock();

    return buf;
}

template<typename KeyType, typename DataType>
inline DataType JasonQt_Map<KeyType, DataType>::operator[](const KeyType &key)
{
    return this->find(key);
}

template<typename KeyType, typename DataType>
JasonQt_Map<KeyType, DataType> &JasonQt_Map<KeyType, DataType>::operator=(const QVector<DataType> &other)
{
    this->copyFrom(other);
    return *this;
}

template<typename KeyType, typename DataType>
void JasonQt_Map<KeyType, DataType>::insert(const DataType &newData, const bool &coverOld)
{
    m_mutex.lock();

    if(coverOld)
    {
        auto it = m_dataFromKey.find(newData.getKey());
        if(it != m_dataFromKey.end())
        {
            *(DataType *)(it.value()) = newData;
        }
        else
        {
            m_data.push_back(newData);
            this->refreshKey();
        }
    }
    else
    {
        m_data.push_back(newData);
        this->refreshKey();
    }

    m_mutex.unlock();
}

template<typename KeyType, typename DataType>
void JasonQt_Map<KeyType, DataType>::clear(void)
{
    m_mutex.lock();

    m_dataFromKey.clear();
    m_data.clear();

    m_mutex.unlock();
}

template<typename KeyType, typename DataType>
int JasonQt_Map<KeyType, DataType>::size(void)
{
    m_mutex.lock();

    const int Size = m_data.size();

    m_mutex.unlock();

    return Size;
}

template<typename KeyType, typename DataType>
DataType JasonQt_Map<KeyType, DataType>::find(const KeyType &key)
{
    DataType buf;

    m_mutex.lock();

    auto Now = m_dataFromKey.find(key);
    if(Now != m_dataFromKey.end())
    {
        buf = *Now.value();
    }

    m_mutex.unlock();

    return buf;
}

template<typename KeyType, typename DataType>
const DataType &JasonQt_Map<KeyType, DataType>::at(const int &index) const
{
    return m_data.at(index);
}

template<typename KeyType, typename DataType>
QVector<DataType> JasonQt_Map<KeyType, DataType>::data(void)
{
    m_mutex.lock();

    auto buf = m_data;

    m_mutex.unlock();

    return buf;
}

template<typename KeyType, typename DataType>
void JasonQt_Map<KeyType, DataType>::copyFrom(const QVector<DataType> &other)
{
    m_mutex.lock();

    this->clear(0);

    m_data = other;

    for(const auto &Now: m_data)
    {
        m_dataFromKey[Now.getKey()] = &Now;
    }

    m_mutex.unlock();
}

template<typename KeyType, typename DataType>
void JasonQt_Map<KeyType, DataType>::foreachData(const std::function<bool (const DataType &)> &each)
{
    m_mutex.lock();

    QVector<DataType> buf(m_data);

    m_mutex.unlock();

    for(const auto &Now: buf)
    {
        if(!each(Now))
        {
            break;
        }
    }
}

template<typename KeyType, typename DataType>
template<typename FindKeyType>
void JasonQt_Map<KeyType, DataType>::foreachData(const std::function<bool (const DataType &)> &each, const FindKeyType &findKey, const std::function<bool (const DataType &, const FindKeyType &)> &find)
{
    QVector<DataType> buf;

    m_mutex.lock();

    for(const auto &Now: m_data)
    {
        if(find(Now, findKey))
        {
            buf.push_back(Now);
        }
    }

    m_mutex.unlock();

    for(const auto &Now: buf)
    {
        if(!each(Now))
        {
            break;
        }
    }
}

template<typename KeyType, typename DataType>
void JasonQt_Map<KeyType, DataType>::clear(int)
{
    m_dataFromKey.clear();
    m_data.clear();
}

template<typename KeyType, typename DataType>
void JasonQt_Map<KeyType, DataType>::refreshKey(void)
{
    m_dataFromKey.clear();

    for(const auto &Now: m_data)
    {
        m_dataFromKey.insert(Now.getKey(), &Now);
    }
}

// JasonQt_AvailableSingleSocketManage
#ifdef QT_NETWORK_LIB
template<typename KeyType>
JasonQt_AvailableSingleSocketManage<KeyType>::JasonQt_AvailableSingleSocketManage(const int &maxIntervalSecond)
{
    this->setMaxInterval(maxIntervalSecond);
}

template<typename KeyType>
void JasonQt_AvailableSingleSocketManage<KeyType>::refreshSocket(const KeyType &key, JasonQt_Tcp::Socket *socket)
{
    m_mutex.lock();

    auto &&currentTime = QDateTime::currentDateTime().toTime_t();
    auto &&currentNode = m_nodes.find(key);
    auto &&currentSocket = m_availableSocket.find(socket);

    if(currentSocket == m_availableSocket.end())
    {
        qDebug("JasonQt_AvailableSocketManageAbstract: error2");
        m_mutex.unlock();
        return;
    }

    if(currentNode != m_nodes.end())
    {
        currentNode->socket = socket;
        currentNode->socketFlag = currentSocket.value();
        currentNode->lastConnectTime = currentTime;
    }
    else
    {
        m_nodes.insert(key, {socket, currentSocket.value(), currentTime});
    }

    m_mutex.unlock();
}

template<typename KeyType>
bool JasonQt_AvailableSingleSocketManage<KeyType>::selectAvailable(const KeyType &key, const std::function<void (JasonQt_Tcp::Socket *)> &send)
{
    m_mutex.lock();

    bool flag = false;
    auto &&currentNode = m_nodes.find(key);

    if(currentNode != m_nodes.end())
    {
        if((QDateTime::currentDateTime().toTime_t() - currentNode.value().lastConnectTime) <= m_maxInterval)
        {
            auto &&currentSocket = m_availableSocket.find(currentNode.value().socket);

            if(currentSocket != m_availableSocket.end())
            {
                if(currentNode.value().socketFlag == currentSocket.value())
                {
                    flag = true;
                    send(currentNode->socket);
                }
            }
        }
    }

    m_mutex.unlock();

    return flag;
}

template<typename KeyType>
JasonQt_AvailableMultiSocketManage<KeyType>::JasonQt_AvailableMultiSocketManage(const int &maxIntervalSecond)
{
    this->setMaxInterval(maxIntervalSecond);
}

template<typename KeyType>
void JasonQt_AvailableMultiSocketManage<KeyType>::refreshSocket(const KeyType &key, JasonQt_Tcp::Socket *socket)
{
    m_mutex.lock();

    auto &&currentTime = QDateTime::currentDateTime().toTime_t();
    auto &&currentNodes = m_nodes.find(key);
    auto &&currentSocket = m_availableSocket.find(socket);

    if(currentSocket == m_availableSocket.end())
    {
        qDebug("JasonQt_AvailableSocketManageAbstract: error2");
        m_mutex.unlock();
        return;
    }

    if(currentNodes != m_nodes.end())
    {
        bool flag = true;

        for(auto node = currentNodes->begin(); !currentNodes->isEmpty() && (node != currentNodes->end()); node++)
        {
            if(node->socket == socket)
            {
                node->socketFlag = currentSocket.value();
                node->lastConnectTime = currentTime;
                flag = false;
            }
            else
            {
                if((currentTime - node->lastConnectTime) > m_maxInterval)
                {
                    currentNodes->erase(node);
                    node = currentNodes->begin();
                }
                else
                {
                    auto &&s = m_availableSocket.find(node->socket);
                    if((s == m_availableSocket.end()) || (*s != node->socketFlag))
                    {
                        currentNodes->erase(node);
                        node = currentNodes->begin();
                    }
                }
            }
        }

        if(flag)
        {
            currentNodes->push_front({socket, currentSocket.value(), currentTime});
        }
    }
    else
    {
        m_nodes.insert(key, {{socket, currentSocket.value(), currentTime}});
    }

    m_mutex.unlock();
}

template<typename KeyType>
int JasonQt_AvailableMultiSocketManage<KeyType>::selectAvailable(const KeyType &key, const std::function<void (JasonQt_Tcp::Socket *)> &send)
{
    m_mutex.lock();

    int flag = 0;
    auto &&currentNodes = m_nodes.find(key);

    if(currentNodes != m_nodes.end())
    {
        for(const auto &node: currentNodes.value())
        {
            if((QDateTime::currentDateTime().toTime_t() - node.lastConnectTime) <= m_maxInterval)
            {
                auto &&currentSocket = m_availableSocket.find(node.socket);

                if(currentSocket != m_availableSocket.end())
                {
                    if(node.socketFlag == currentSocket.value())
                    {
                        flag++;
                        send(node.socket);
                    }
                }
            }
        }
    }

    m_mutex.unlock();

    return flag;
}

#endif

// JasonQt_CacheManage
template<typename KeyType, typename DataType>
JasonQt_CacheManage<KeyType, DataType>::JasonQt_CacheManage(const std::function<std::pair<bool, DataType>(const KeyType &key)> &updateSource)
{
    this->setUpdateSource(updateSource);
}

template<typename KeyType, typename DataType>
void JasonQt_CacheManage<KeyType, DataType>::update(const KeyType &key, const DataType &data)
{
    m_mutex.lock();

    m_data[key] = data;

    m_mutex.unlock();
}

template<typename KeyType, typename DataType>
bool JasonQt_CacheManage<KeyType, DataType>::delete_(const KeyType &key)
{
    m_mutex.lock();

    const auto &&now = m_data.find(key);
    bool flag;

    if(now != m_data.end())
    {
        flag = true;
        m_data.erase(now);
    }
    else
    {
        flag = false;
    }

    m_mutex.unlock();

    return flag;
}

template<typename KeyType, typename DataType>
void JasonQt_CacheManage<KeyType, DataType>::clear(void)
{
    m_mutex.lock();

    m_data.clear();

    m_mutex.unlock();
}

template<typename KeyType, typename DataType>
bool JasonQt_CacheManage<KeyType, DataType>::contains(const KeyType &key)
{
    const auto &&buf = this->select(key);
    return buf.first;
}

template<typename KeyType, typename DataType>
bool JasonQt_CacheManage<KeyType, DataType>::select(const KeyType &key, DataType &target)
{
    const auto &&buf = this->select(key);
    if(buf.first)
    {
        target = buf.second;
    }
    return buf.first;
}

template<typename KeyType, typename DataType>
std::pair<bool, DataType> JasonQt_CacheManage<KeyType, DataType>::select(const KeyType &key)
{
    m_mutex.lock();

    const auto &&now = m_data.find(key);

    if(now != m_data.end())
    {
        const std::pair<bool, DataType> &&buf = { true, *now };

        m_mutex.unlock();

        return buf;
    }
    else
    {
        if(m_updateSource)
        {
            const auto &&buf = (*m_updateSource)(key);
            if(buf.first)
            {
                m_data[key] = buf.second;

                m_mutex.unlock();

                return buf;
            }
        }

        m_mutex.unlock();

        return { false, DataType() };
    }
}

#endif//__JasonQt_Container_h__
