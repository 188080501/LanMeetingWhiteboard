#ifndef __LanMeetingWhiteboardAgreement_h__
#define __LanMeetingWhiteboardAgreement_h__

// Jdn lib impor
#include "Jdn.h"

#define WhiteboardLanUdpPort                    33178
#define WhiteboardLanTcpPort                    33179
#define WhiteboardMaxSourceSize                 3

namespace Agreement
{

// Num define
enum NumEnum
{
    ServerInformationDataNum                = 1005,
    CanvasSourceDataNum                     = 1010,

    GetCanvasAllRequestNum                  = 2005,
    ClientPingRequestNum                    = 2010,

    PaintDataFromClientRequestNum           = 3010,
    PaintDataFromServerRequestNum           = 3015,
    ImageDataFromServerRequestNum           = 3020,

    BackToHistoryRequestNum                 = 4005,
    BackToFutureRequestNum                  = 4010
};

enum PaintGroupEnum
{
    UnKnowPaint                             = 0,
    PaintPoint                              = 5,
    PaintLine                               = 10,
    PaintRectangle                          = 15,
    PaintEllipse                            = 20,
    PaintArrow                              = 25,

    PaintStar                               = 55,
    PaintHeart                              = 60,

    PaintText                               = 105,
    PaintImage                              = 110,

    PaintEraser                             = 155,
};

// Package define
class ServerInformationDataPackage
{
public:
    quint32 ip;
    QString hostName;
    QString serverName;
    QString password;
    bool clientEdit = false;

    void toVariantList(QVariantList &target) const
    {
        target.push_back(ip);
        target.push_back(hostName);
        target.push_back(serverName);
        target.push_back(password);
        target.push_back(clientEdit);
    }

    bool fromVariantList(const QVariantList &source)
    {
        ip = source.at(0).toUInt();
        hostName = source.at(1).toString();
        serverName = source.at(2).toString();
        password = source.at(3).toString();
        clientEdit = source.at(4).toBool();

        return true;
    }

    JdnMultiDataAgreement(ServerInformationDataNum, true, 5)
};

class ClientPingRequestPackage
{
public:
    quint32 clientIP = 0;
    quint32 serverIP = 0;

    JdnSingleDataAgreement(ClientPingRequestNum, true)
};

}

#endif//__LanMeetingWhiteboardAgreement_h__
