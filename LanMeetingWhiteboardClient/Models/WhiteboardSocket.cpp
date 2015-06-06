#include "WhiteboardSocket.h"

WhiteboardSocket::WhiteboardSocket(WhiteboardList *whiteboardList, WhiteboardCanvasPainter *whiteboardCanvasPainter):
    m_whiteboardList(whiteboardList),
    m_whiteboardCanvasPainter(whiteboardCanvasPainter),
    m_lanSocket(&whiteboardList->m_lanSearcher, WhiteboardLanTcpPort)
{
    this->onLanSearcherSend();
    this->onLanSocketAccepted();
    this->onPaint();

    m_lanSocket.begin();
}

void WhiteboardSocket::startServer(const QString &serverName, const QString &password, const bool &clientEdit)
{
    m_serverModeOpen = true;

    m_serverModeData.serverName = serverName;
    m_serverModeData.password = password;
    m_serverModeData.clientEdit = clientEdit;
}

void WhiteboardSocket::startClient(const QString &ip)
{
    Jdn::Set data;

    data.push_back(Agreement::GetCanvasAllRequestNum);

    m_lanSocket.send(data, { QHostAddress(ip) });

    m_clientModeServerIp = QHostAddress(ip).toIPv4Address();
}

void WhiteboardSocket::stop(void)
{
    m_serverModeOpen = false;
    m_clientModeServerIp = 0;
}

bool WhiteboardSocket::isServer(void)
{
    return m_serverModeOpen;
}

bool WhiteboardSocket::isClient(void)
{
    return m_clientModeServerIp != 0;
}

void WhiteboardSocket::sendImageToClient(const int &sourceIndex, const QByteArray &imageData)
{
    Jdn::Set data;
    data.push_back(Agreement::ImageDataFromServerRequestNum, imageData);
    data.push_back(sourceIndex);
    m_lanSocket.send(data, m_whiteboardList->availableClients());
}

void WhiteboardSocket::sendBackToHistory(const int &sourceIndex)
{
    Jdn::Set data;
    data.push_back(Agreement::BackToHistoryRequestNum, sourceIndex);
    m_lanSocket.send(data, { QHostAddress(m_clientModeServerIp) } );
}

void WhiteboardSocket::sendBackToFuture(const int &sourceIndex)
{
    Jdn::Set data;
    data.push_back(Agreement::BackToFutureRequestNum, sourceIndex);
    m_lanSocket.send(data, { QHostAddress(m_clientModeServerIp) } );
}

void WhiteboardSocket::onLanSearcherSend(void)
{
    m_whiteboardList->setOnLanSearcherSendCallback([&](Jdn::Set &appendData)
    {
        if(this->isServer())
        {
            Agreement::ServerInformationDataPackage package;

            package.ip = m_whiteboardList->m_lanSearcher.currentAddressEntry().ip().toIPv4Address();
            package.hostName = JasonQt_Net::getDeviceName();
            package.serverName = m_serverModeData.serverName;
            package.password = m_serverModeData.password;
            package.clientEdit = m_serverModeData.clientEdit;

            appendData.push_back_multi_data(package);
        }
        else if(this->isClient())
        {
            Agreement::ClientPingRequestPackage package;

            package.clientIP = m_whiteboardList->m_lanSearcher.currentAddressEntry().ip().toIPv4Address();
            package.serverIP = QHostAddress(m_clientModeServerIp).toIPv4Address();

            appendData.push_back_single_data(package);
        }
    });
}

void WhiteboardSocket::onLanSocketAccepted(void)
{
    m_lanSocket.setOnAcceptedCallback([&](Jdn::Set &accepted, Jdn::Set &send)
    {
        while(accepted.next())
        {
            switch(accepted.currentNodeNum())
            {
                case Agreement::CanvasSourceDataNum:
                {
                    if(accepted.currentNodeType() == Jdn::WithByteArrayNode)
                    {
                        const auto imageData = accepted.currentNodeQByteArray();

                        if(accepted.pop_front() && (accepted.currentNodeType() == Jdn::OnlyNumNode))
                        {
                            const auto sourceIndex = accepted.currentNodeNum();

                            m_whiteboardCanvasPainter->sourceLoadFromByteArray(sourceIndex, imageData);
                        }
                    }
                    break;
                }
                case Agreement::GetCanvasAllRequestNum:
                {
                    for(int now = 0; now < WhiteboardMaxSourceSize; now++)
                    {
                        const auto &&buf = m_whiteboardCanvasPainter->sourceToByteArray(now);
                        send.push_back(Agreement::CanvasSourceDataNum, buf);
                        send.push_back(now);
                    }
                    break;
                }
                case Agreement::PaintDataFromClientRequestNum:
                {
                    if(accepted.currentNodeType() == Jdn::WithByteArrayNode)
                    {
                        const auto imageData = accepted.currentNodeQByteArray();

                        if(accepted.pop_front() && (accepted.currentNodeType() == Jdn::OnlyNumNode))
                        {
                            const auto sourceIndex = accepted.currentNodeNum();

                            m_whiteboardCanvasPainter->sourcePaintFromByteArray(sourceIndex, imageData);

                            if(accepted.pop_front())
                            {
                                Jdn::Set data;
                                data.push_back(Agreement::PaintDataFromServerRequestNum, imageData);
                                m_lanSocket.send(data, m_whiteboardList->availableClients(quint32(accepted.currentNodeNum())));
                            }
                        }

                    }
                    break;
                }
                case Agreement::PaintDataFromServerRequestNum:
                {
                    if(accepted.currentNodeType() == Jdn::WithByteArrayNode)
                    {
                        const auto imageData = accepted.currentNodeQByteArray();

                        if(accepted.pop_front() && (accepted.currentNodeType() == Jdn::OnlyNumNode))
                        {
                            const auto sourceIndex = accepted.currentNodeNum();

                            m_whiteboardCanvasPainter->sourcePaintFromByteArray(sourceIndex, imageData);
                        }
                    }
                    break;
                }
                case Agreement::ImageDataFromServerRequestNum:
                {
                    if(accepted.currentNodeType() == Jdn::WithByteArrayNode)
                    {
                        const auto imageData = accepted.currentNodeQByteArray();

                        if(accepted.pop_front() && (accepted.currentNodeType() == Jdn::OnlyNumNode))
                        {
                            const auto sourceIndex = accepted.currentNodeNum();

                            m_whiteboardCanvasPainter->sourceLoadFromByteArray(sourceIndex, imageData);
                        }
                    }
                    break;
                }

                case Agreement::BackToHistoryRequestNum:
                {
                    m_whiteboardCanvasPainter->backToHistory();
                    break;
                }
                case Agreement::BackToFutureRequestNum:
                {
                    m_whiteboardCanvasPainter->backToFuture();
                    break;
                }
            }
        }
    });
}

void WhiteboardSocket::onPaint(void)
{
    m_onPaintCallback = [&](const int &sourceIndex, const QByteArray &imageData)
    {
        if(this->isServer())
        {
            Jdn::Set data;
            data.push_back(Agreement::PaintDataFromServerRequestNum, imageData);
            data.push_back(sourceIndex);
            m_lanSocket.send(data, m_whiteboardList->availableClients());
        }
        else if(this->isClient())
        {
            Jdn::Set data;
            data.push_back(Agreement::PaintDataFromClientRequestNum, imageData);
            data.push_back(sourceIndex);
            data.push_back(int32_t(m_whiteboardList->m_lanSearcher.currentAddressEntry().ip().toIPv4Address()));
            m_lanSocket.send(data, { QHostAddress(m_clientModeServerIp) } );
        }
    };

    m_whiteboardCanvasPainter->setOnPaintCallback(m_onPaintCallback);
}
