#include "WhiteboardList.h"

WhiteboardList::WhiteboardList(void):
    m_lanSearcher(WhiteboardLanUdpPort)
{
    this->startTimer(5000);

    m_timer.setInterval(800);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, [=]()
    {
        emit serverListChange();
    });

    m_lanSearcher.setOnSendCallback([&](Jdn::Set &appendData)
    {
        if(m_onLanSearcherSendCallback)
        {
            (*m_onLanSearcherSendCallback)(appendData);
        }
    });

    this->onLanSearcherAccepted();

    if(!m_lanSearcher.begin(true))
    {
        QTimer::singleShot(2000, [=]()
        {
            emit beginFail();
        });
    }
}

QList<QHostAddress> WhiteboardList::availableClients(const quint32 &exception)
{
    const auto &&currentTime = QDateTime::currentDateTime().toTime_t();
    QList<QHostAddress> buf;

    for(auto it = m_availableClients.begin(); it != m_availableClients.end(); it++)
    {
        if(((currentTime - it.value()) < 10) && (it.key() != exception))
        {
            buf.push_back(QHostAddress(it.key()));
        }
    }

    return buf;
}

bool WhiteboardList::containsServer(const quint32 &ip)
{
    return m_availableServers.contains(ip);
}

QJsonObject WhiteboardList::server(const QString &ip)
{
    QJsonObject buf;

    buf["ip"] = ip;
    buf["hostName"] = m_availableServers[QHostAddress(ip).toIPv4Address()].second.hostName;
    buf["serverName"] = m_availableServers[QHostAddress(ip).toIPv4Address()].second.serverName;
    buf["password"] = m_availableServers[QHostAddress(ip).toIPv4Address()].second.password;
    buf["clientEdit"] = m_availableServers[QHostAddress(ip).toIPv4Address()].second.clientEdit;

    return buf;
}

QJsonArray WhiteboardList::availableServers(void)
{
    QJsonArray data;
    QJsonObject buf;
    const auto &&currentTime = QDateTime::currentDateTime().toTime_t();

    for(const auto &now: m_availableServers)
    {
        if((currentTime - now.first) > 10) { continue; }

        buf["ip"] = QHostAddress(now.second.ip).toString();
        buf["hostName"] = now.second.hostName;
        buf["serverName"] = now.second.serverName;
        buf["password"] = now.second.password;
        buf["clientEdit"] = now.second.clientEdit;
        data.push_back(buf);
    }

    return data;
}

void WhiteboardList::timerEvent(QTimerEvent *)
{
    emit serverListChange();
}

void WhiteboardList::onLanSearcherAccepted(void)
{
    connect(&m_lanSearcher, &JasonQt_Lan::LanSearcher::disConnect, [&](const QHostAddress &address)
    {
        if(m_availableServers.contains(address.toIPv4Address()))
        {
            m_availableServers.erase(m_availableServers.find(address.toIPv4Address()));
            m_timer.start();
        }
    });

    m_lanSearcher.setOnAcceptedCallback([&](Jdn::Set &data)
    {
        if(data.size())
        {
            do
            {
                switch(data.currentNodeNum())
                {
                    case Agreement::ServerInformationDataPackage::packageNum():
                    {
                        Agreement::ServerInformationDataPackage package;

                        if(data.copy_current_from_multi_data(package))
                        {
                            if(!m_availableServers.contains(package.ip))
                            {
                                m_timer.start();
                            }

                            qDebug() << "ServerPing from:" << QHostAddress(package.ip).toString() << ", IpName:" << package.hostName << ", ServerName:" << package.serverName;
                            if(QHostAddress(package.ip) != m_lanSearcher.currentAddressEntry().ip())
                            {
                                m_availableServers[package.ip].first = QDateTime::currentDateTime().toTime_t();
                                m_availableServers[package.ip].second = package;
                            }
                        }
                        break;
                    }
                    case Agreement::ClientPingRequestPackage::packageNum():
                    {
                        Agreement::ClientPingRequestPackage package;

                        if(data.copy_current_from_single_data(package))
                        {
                            if(package.serverIP == m_lanSearcher.currentAddressEntry().ip().toIPv4Address())
                            {
                                qDebug() << "ClientPing from:" << QHostAddress(package.clientIP).toString();
                                m_availableClients[package.clientIP] = QDateTime::currentDateTime().toTime_t();
                            }
                        }
                        break;
                    }
                }
            }
            while(data.pop_front());
        }
    });
}
