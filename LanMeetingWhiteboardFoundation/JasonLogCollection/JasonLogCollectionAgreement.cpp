#include "JasonLogCollectionAgreement.h"

using namespace JasonLogCollectionAgreement;

JasonQt_Tcp::SocketSettings JasonLogCollectionAgreement::socketSettings;
JasonQt_Tcp::ServerSettings JasonLogCollectionAgreement::serverSettings;
JasonQt_Tcp::ClientSettings JasonLogCollectionAgreement::clientSettings;

auto *JasonLogCollectionAgreement_Socket_Settings_Begin = new JasonQt_BeforMain([]()
{
    serverSettings.setConnectPort(JasonLogCollectionAgreementServerPort);
    serverSettings.setJdeKey( { JasonLogCollectionAgreementJdePublicKey } );

    clientSettings.setConnectAddress(JasonLogCollectionAgreementServerAddress);
    clientSettings.setConnectPort(JasonLogCollectionAgreementServerPort);
    clientSettings.setJdeKey( { JasonLogCollectionAgreementJdePublicKey } );
});

// DataPack
LogDataPackage::LogDataPackage(const QString &informationKey_, const QString &append_)
{
    informationKey = informationKey_;
    append = append_;
}

LogDataPackage::LogDataPackage(const QString &informationKey_, const QString &append_, const quint32 time_, const quint32 IP_)
{
    informationKey = informationKey_;
    append = append_;
    time = time_;
    IP = IP_;
}

InformationKeyDataPackage::InformationKeyDataPackage(const QString &informationKey_, const QString &getKey_)
{
    informationKey = informationKey_;
    getKey = getKey_;
}

Information_DataPackage::Information_DataPackage(const QString &InformationKey_, const QString &GetKey_,
                                           const bool &RunKey_Use_, const QString &RunKey_, const bool &VersionKey_Use_, const qint32 &VersionKey_, const bool &DateKey_Use_, const quint32 DateKey_)
{
    informationKey = InformationKey_;
    getKey = GetKey_;

    runKeyUse = RunKey_Use_;
    runKey = RunKey_;
    versionKeyUse = VersionKey_Use_;
    versionKey = VersionKey_;
    dateKeyUse = DateKey_Use_;
    dateKey = DateKey_;
}

bool Information_DataPackage::operator<(const Information_DataPackage &Other) const
{
    return this->informationKey < Other.informationKey;
}

// RequestPackage-Question
QuestionUploadLogRequestPackage::QuestionUploadLogRequestPackage(const QString &InformationKey_, const QString &Append_)
{
    informationKey = InformationKey_;
    append = Append_;
}

QuestionSelectInformationRequestPackage::QuestionSelectInformationRequestPackage(const QString &InformationKey_, const QString &GetKey_)
{
    informationKey = InformationKey_;
    getKey = GetKey_;
}

// RequestPackage-Admin
AdminSelectLogRequestPackage::AdminSelectLogRequestPackage(const QString &AdminKey_, const QString &InformationKey_, const qint32 &Count_)
{
    adminKey = AdminKey_;
    informationKey = InformationKey_;
    count = Count_;
}

AdminDeleteLogRequestPackage::AdminDeleteLogRequestPackage(const QString &AdminKey_, const LogDataPackage &Log)
{
     adminKey = AdminKey_;
    *((LogDataPackage *)this) = Log;
}

AdminSelectInformationRequestPackage::AdminSelectInformationRequestPackage(const QString &AdminKey_, const QString &InformationKey_)
{
    adminKey = AdminKey_;
    informationKey = InformationKey_;
}

AdminInsertInformationRequestPackage::AdminInsertInformationRequestPackage(const QString &AdminKey_, const Information_DataPackage &Information)
{
    adminKey = AdminKey_;
    *((Information_DataPackage *)this) = Information;
}

AdminDeleteInformationRequestPackage::AdminDeleteInformationRequestPackage(const QString &AdminKey_, const InformationKeyDataPackage &InformationKey)
{
    adminKey = AdminKey_;
    *((InformationKeyDataPackage *)this) = InformationKey;
}

AdminUpdateInformationRequestPackage::AdminUpdateInformationRequestPackage(const QString &AdminKey_, const Information_DataPackage &Information)
{
    adminKey = AdminKey_;
    *((Information_DataPackage *)this) = Information;
}
