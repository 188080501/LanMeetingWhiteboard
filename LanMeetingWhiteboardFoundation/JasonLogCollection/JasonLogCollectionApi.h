#ifndef __Api_h__
#define __Api_h__

// Qt lib import
#include <QtConcurrent>
#include <QMessageBox>

// JasonLogCollection lib import
#include "JasonLogCollectionAgreement.h"

namespace JasonLogCollectionApi
{

class Api: public QObject
{
    Q_OBJECT

private:
    JasonQt_Tcp::ClientManage *m_client;

public:
    Api(QObject *parent = NULL);

    ~Api(void);

    static void begin(void);

    static void end(void);

public:
    bool ping(void);

    bool questionUplodaLog(const JasonLogCollectionAgreement::QuestionUploadLogRequestPackage &requestPackage);

    bool questionSelectInformation(const JasonLogCollectionAgreement::QuestionSelectInformationRequestPackage &requestPackage, JasonLogCollectionAgreement::Information_DataPackage &target, QString &target2);

    bool adminDeleteLog(const JasonLogCollectionAgreement::AdminDeleteLogRequestPackage &requestPackage);

    bool adminSelectLog(const JasonLogCollectionAgreement::AdminSelectLogRequestPackage &requestPackage, QVector<JasonLogCollectionAgreement::LogDataPackage> &target);

    bool adminInsertInformation(const JasonLogCollectionAgreement::AdminInsertInformationRequestPackage &requestPackage, const QString &append);

    bool adminDeleteInformation(const JasonLogCollectionAgreement::AdminDeleteInformationRequestPackage &requestPackage);

    bool adminUpdateInformation(const JasonLogCollectionAgreement::AdminUpdateInformationRequestPackage &requestPackage, const QString &append);

    bool adminSelectInformation(const JasonLogCollectionAgreement::AdminSelectInformationRequestPackage &requestPackage, QVector<JasonLogCollectionAgreement::Information_DataPackage> &target, QVector<QString> &target2);
};

extern Api *api;

void standardHandleProcess(const QString &informationKey, const QString &logAppend);

void standardHandleProcess(const QString &informationKey, const QString &logAppend, const QString &informationGetKey, const QString &runKey, const qint32 versionKey, const bool &closeAppOnSelectInformationFail = false);

}

#endif//__Api_h__
