#ifndef __WhiteboardList_h__
#define __WhiteboardList_h__

// JasonQt lib import
#include "JasonQt_Lan.h"

// LanMeetingWhiteboard lib import
#include "LanMeetingWhiteboardAgreement.hpp"

class WhiteboardSocket;

class WhiteboardList: public QObject
{
    Q_OBJECT
    friend class WhiteboardSocket;

private:
    JasonQt_Lan::LanSearcher m_lanSearcher;
    QTimer m_timer;

    QMap<quint32, std::pair<quint32, Agreement::ServerInformationDataPackage> > m_availableServers;
    QMap<quint32, quint32> m_availableClients;

    PtrPropertyDeclare(std::function<void(Jdn::Set &appendData)>, onLanSearcherSendCallback, setOnLanSearcherSendCallback, = NULL)

public:
    WhiteboardList(void);

    QList<QHostAddress> availableClients(const quint32 &exception = 0);

    bool containsServer(const quint32 &ip);

public slots:
    QJsonObject server(const QString &ip);

    QJsonArray availableServers(void);

private:
    void timerEvent(QTimerEvent *);

    void onLanSearcherAccepted(void);

signals:
    void serverListChange(void);

    void beginFail(void);
};

#endif//__WhiteboardList_h__
