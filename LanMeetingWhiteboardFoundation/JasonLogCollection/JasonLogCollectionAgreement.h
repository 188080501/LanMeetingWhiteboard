#ifndef __JasonLogCollectionAgreement_h__
#define __JasonLogCollectionAgreement_h__

// JasonQt lib import
#include "JasonQt_Container.h"
#include "JasonQt_TcpServer.h"
#include "JasonQt_TcpClient.h"

#define JasonLogCollectionAgreementServerAddress                    "JasonLogCollection.JasonServer.com"
#define JasonLogCollectionAgreementServerPort                       23410
#define JasonLogCollectionAgreementJdePublicKey                     "SmFzb25Mb2dDb2xsZWN0aW9u"

namespace JasonLogCollectionAgreement
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Socket define ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern JasonQt_Tcp::SocketSettings socketSettings;
extern JasonQt_Tcp::ServerSettings serverSettings;
extern JasonQt_Tcp::ClientSettings clientSettings;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ enum define ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum NumEnum
{
    PingNum                                                     = 0,
    OKNum                                                       = 5,
    ErrorNum                                                    = 10,

    LogDataNum                                                  = 505,
    InformationKeyDataNum                                       = 510,
    InformationDataNum                                          = 515,

    QuestionUploadLogRequestNum                                 = 1005,
    QuestionSelectInformationRequestNum                         = 1105,

    AdminDeleteLogRequestNum                                    = 2010,
    AdminSelectLogRequestNum                                    = 2020,
    AdminInsertInformationRequestNum                            = 2105,
    AdminDeleteInformationRequestNum                            = 2110,
    AdminUpdateInformationRequestNum                            = 2115,
    AdminSelectInformationRequestNum                            = 2120
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Package define ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma pack(push)
#pragma pack(1)

// Foundation
class AdminKeyAncestry
{
public:
    JasonQt_HashChar adminKey;
};

// DataPack
class LogDataPackage
{
public:
    JasonQt_Char<50, false> informationKey;
    JasonQt_Char<100, true> append;

    quint32 time = 0;
    quint32 IP = 0;

    LogDataPackage(void) = default;

    LogDataPackage(const QString &informationKey, const QString &append);

    LogDataPackage(const QString &informationKey, const QString &append, const quint32 time, const quint32 IP);

    JdnSingleDataAgreement(LogDataNum, false)
};

class InformationKeyDataPackage
{
public:
    JasonQt_Char<50, false> informationKey;
    JasonQt_Char<50, false> getKey;

    InformationKeyDataPackage(void) = default;

    InformationKeyDataPackage(const QString &informationKey, const QString &getKey);

    JdnSingleDataAgreement(InformationKeyDataNum, false)
};

class Information_DataPackage:
    public InformationKeyDataPackage
{
public:
    bool    runKeyUse = false;
    JasonQt_Char<50, false> runKey;

    bool    versionKeyUse = false;
    qint32  versionKey = 0;

    bool    dateKeyUse = false;
    quint32 dateKey = 0;

    Information_DataPackage(void) = default;

    Information_DataPackage(const QString &informationKey, const QString &getKey,
                         const bool &runKeyUse, const QString &runKey, const bool &versionKeyUse, const qint32 &versionKey, const bool &dateKeyUse, const quint32 dateKey);

    bool operator<(const Information_DataPackage &Other) const;

    JdnSingleDataAgreement(InformationDataNum, false)
};

// RequestPackage-Question
class QuestionUploadLogRequestPackage:
    public LogDataPackage
{
public:
    QuestionUploadLogRequestPackage(void) = default;

    QuestionUploadLogRequestPackage(const QString &informationKey, const QString &append);

    JdnSingleDataAgreement(QuestionUploadLogRequestNum, false)
};

class QuestionSelectInformationRequestPackage:
    public InformationKeyDataPackage
{
public:
    QuestionSelectInformationRequestPackage(void) = default;

    QuestionSelectInformationRequestPackage(const QString &informationKey, const QString &getKey);

    JdnSingleDataAgreement(QuestionSelectInformationRequestNum, false)
};

// RequestPackage-Admin
class AdminDeleteLogRequestPackage:
    public AdminKeyAncestry,
    public LogDataPackage
{
public:
    AdminDeleteLogRequestPackage(void) = default;

    AdminDeleteLogRequestPackage(const QString &adminKey, const LogDataPackage &Log);

    JdnSingleDataAgreement(AdminDeleteLogRequestNum, true)
};

class AdminSelectLogRequestPackage:
    public AdminKeyAncestry
{
public:
    JasonQt_Char<51, false> informationKey;
    qint32 count = 200;

    AdminSelectLogRequestPackage(void) = default;

    AdminSelectLogRequestPackage(const QString &adminKey, const QString &informationKey, const qint32 &count = 200);

    JdnSingleDataAgreement(AdminSelectLogRequestNum, true)
};

class AdminInsertInformationRequestPackage:
    public AdminKeyAncestry,
    public Information_DataPackage
{
public:
    AdminInsertInformationRequestPackage(void) = default;

    AdminInsertInformationRequestPackage(const QString &adminKey, const Information_DataPackage &Information);

    JdnSingleDataAgreement(AdminInsertInformationRequestNum, true)
};

class AdminDeleteInformationRequestPackage:
    public AdminKeyAncestry,
    public InformationKeyDataPackage
{
public:
    AdminDeleteInformationRequestPackage(void) = default;

    AdminDeleteInformationRequestPackage(const QString &adminKey, const InformationKeyDataPackage &informationKey);

    JdnSingleDataAgreement(AdminDeleteInformationRequestNum, true)
};

class AdminUpdateInformationRequestPackage:
    public AdminKeyAncestry,
    public Information_DataPackage
{
public:
    AdminUpdateInformationRequestPackage(void) = default;

    AdminUpdateInformationRequestPackage(const QString &adminKey, const Information_DataPackage &Information);

    JdnSingleDataAgreement(AdminUpdateInformationRequestNum, true)
};

class AdminSelectInformationRequestPackage:
    public AdminKeyAncestry
{
public:
    JasonQt_Char<51, false> informationKey;

    AdminSelectInformationRequestPackage(void) = default;

    AdminSelectInformationRequestPackage(const QString &adminKey, const QString &informationKey);

    JdnSingleDataAgreement(AdminSelectInformationRequestNum, true)
};

#pragma pack(pop)

}

#endif//__JasonLogCollectionAgreement_h__
