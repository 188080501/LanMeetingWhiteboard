#ifndef __WhiteboardSocket_h__
#define __WhiteboardSocket_h__

// LanMeetingWhiteboard lib import
#include "WhiteboardList.h"
#include "WhiteboardCanvas.h"

class WhiteboardSocket: public QObject
{
    Q_OBJECT
    friend class WhiteboardCanvasPainter;

private:
    WhiteboardList *m_whiteboardList;
    WhiteboardCanvasPainter *m_whiteboardCanvasPainter;

    JasonQt_Lan::LanSocket m_lanSocket;

    bool m_serverModeOpen = false;
    Agreement::ServerInformationDataPackage m_serverModeData;

    quint32 m_clientModeServerIp;

    std::function<void(const int &, const QByteArray &)> m_onPaintCallback;

public:
    WhiteboardSocket(WhiteboardList *whiteboardList, WhiteboardCanvasPainter *whiteboardCanvasPainter);

public slots:
    void startServer(const QString &serverName, const QString &password, const bool &clientEdit);

    void startClient(const QString &ip);

    void stop(void);

    bool isServer(void);

    bool isClient(void);

    void sendImageToClient(const int &sourceIndex, const QByteArray &imageData);

    void sendBackToHistory(const int &sourceIndex);

    void sendBackToFuture(const int &sourceIndex);

private:
    void onLanSearcherSend(void);

    void onLanSocketAccepted(void);

    void onPaint(void);
};

#endif//__WhiteboardSocket_h__
