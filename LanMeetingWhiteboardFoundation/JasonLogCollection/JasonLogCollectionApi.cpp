#include "JasonLogCollectionApi.h"

using namespace JasonLogCollectionAgreement;
using namespace JasonLogCollectionApi;

// Api_Class
Api *JasonLogCollectionApi::api = NULL;

Api::Api(QObject *parent):
    QObject(parent)
{
    m_client = new JasonQt_Tcp::ClientManage(clientSettings, socketSettings);
}

Api::~Api(void)
{
    m_client->deleteLater();
}

void Api::begin(void)
{
    if(api)
    {
        qDebug("Api_Class::Begin: error");
        delete api;
    }
    api = new Api;
}

void Api::end(void)
{
    if(!api)
    {
        qDebug("Api_Class::End: error");
    }
    else
    {
        delete api;
        api = NULL;
    }
}

bool Api::ping(void)
{
    Jdn::Set send;
    send.push_back(PingNum);
    return JasonQt_Tcp::standardHandleProcess(*m_client, send, PingNum, ErrorNum) == "OK";
}

bool Api::questionUplodaLog(const QuestionUploadLogRequestPackage &requestPackage)
{
    return JasonQt_Tcp::standardHandleProcess(*m_client, requestPackage, OKNum, ErrorNum) == "OK";
}

bool Api::questionSelectInformation(const QuestionSelectInformationRequestPackage &requestPackage, Information_DataPackage &target, QString &target2)
{
    Jdn::Set send, accepted;
    send.push_back_single_data(requestPackage);

    if(JasonQt_Tcp::standardHandleProcess(*m_client, send, accepted, OKNum, ErrorNum) == "OK")
    {
        auto Flag = accepted.copy_current_from_single_data(target);
        accepted.pop_front();
        target2 = accepted.currentNodeQString();
        return Flag;
    }
    return false;
}

bool Api::adminDeleteLog(const AdminDeleteLogRequestPackage &requestPackage)
{
    return JasonQt_Tcp::standardHandleProcess(*m_client, requestPackage, OKNum, ErrorNum) == "OK";
}

bool Api::adminSelectLog(const AdminSelectLogRequestPackage &requestPackage, QVector<LogDataPackage> &target)
{
    Jdn::Set send, accepted;
    LogDataPackage buf;
    send.push_back_single_data(requestPackage);

    if(JasonQt_Tcp::standardHandleProcess(*m_client, send, accepted, OKNum, ErrorNum) == "OK")
    {
        while(accepted.next())
        {
            if(accepted.copy_current_from_single_data(buf))
            {
                target.push_back(buf);
            }
        }

        std::sort(target.begin(), target.end(), [](const LogDataPackage &A,
                                                   const LogDataPackage &B)
        {
            if(A.time != B.time)
            {
                return A.time > B.time;
            }
            if(A.informationKey != B.informationKey)
            {
                return A.informationKey < B.informationKey;
            }
            if(A.IP != B.IP)
            {
                return A.IP < B.IP;
            }
            return A.append < B.append;
        });

        return true;
    }
    return false;
}

bool Api::adminInsertInformation(const AdminInsertInformationRequestPackage &requestPackage, const QString &append)
{
    Jdn::Set send;
    send.push_back_single_data(requestPackage);
    send.push_back(requestPackage.packageNum(), append);
    return JasonQt_Tcp::standardHandleProcess(*m_client, send, OKNum, ErrorNum) == "OK";
}

bool Api::adminDeleteInformation(const AdminDeleteInformationRequestPackage &requestPackage)
{
    return JasonQt_Tcp::standardHandleProcess(*m_client, requestPackage, OKNum, ErrorNum) == "OK";
}

bool Api::adminUpdateInformation(const AdminUpdateInformationRequestPackage &requestPackage, const QString &append)
{
    Jdn::Set send;
    send.push_back_single_data(requestPackage);
    send.push_back(requestPackage.packageNum(), append);
    return JasonQt_Tcp::standardHandleProcess(*m_client, send, OKNum, ErrorNum) == "OK";
}

bool Api::adminSelectInformation(const AdminSelectInformationRequestPackage &requestPackage, QVector<Information_DataPackage> &target, QVector<QString> &target2)
{
    target.clear();
    target2.clear();

    Jdn::Set send, accepted;
    Information_DataPackage buf;
    QString buf2;
    send.push_back_single_data(requestPackage);

    if(JasonQt_Tcp::standardHandleProcess(*m_client, send, accepted, OKNum, ErrorNum) == "OK")
    {
        QMap<Information_DataPackage, QString> buf3;

        while(accepted.next())
        {
            if(accepted.copy_current_from_single_data(buf))
            {
                accepted.next();
                buf2 = accepted.currentNodeQString();

                buf3.insert(buf, buf2);
            }
        }

        for(auto it = buf3.begin(); it != buf3.end(); it++)
        {
            target.push_back(it.key());
            target2.push_back(it.value());
        }

        return true;
    }
    return false;
}

// StandardHandleProcess
void JasonLogCollectionApi::standardHandleProcess(const QString &informationKey, const QString &logAppend)
{
    QtConcurrent::run([=]()
    {
        Api::begin();

        if(api->questionUplodaLog({informationKey, logAppend}))
        {
            qDebug("UploadLog finish.");
        }
        else
        {
            qDebug("UploadLog fail.");
        }

        Api::end();
    });
}

void JasonLogCollectionApi::standardHandleProcess(const QString &informationKey, const QString &logAppend, const QString &informationGetKey, const QString &runKey, const qint32 versionKey, const bool &closeAppOnSelectInformationFail)
{
    QtConcurrent::run([=]()
    {
        Api::begin();

        if(api->questionUplodaLog({informationKey, logAppend}))
        {
            qDebug("UploadLog finish.");
        }
        else
        {
            qDebug("UploadLog fail.");
        }

        Information_DataPackage informatin;
        QString append;

        try
        {
            if(api->questionSelectInformation({informationKey, informationGetKey}, informatin, append))
            {
                if(informatin.runKeyUse && (informatin.runKey != runKey))
                {
                    JasonQt_ShowInformationMessageBoxFromOtherThread::show("Message", QStringLiteral("程序出现内部错误\n请咨询程序开发者以解决此问题."));
                    throw QString("RunKey different, Ready exit app.");
                }
                if(informatin.versionKeyUse && (informatin.versionKey > versionKey))
                {
                    JasonQt_ShowInformationMessageBoxFromOtherThread::show("Message", QStringLiteral("您使用的程序版本过低，请升级\n当前版本：%1\n需要版本：%2").arg(versionKey).arg(informatin.versionKey));
                    throw QString("VersionKey lower, Ready exit app.");
                }
                if(informatin.dateKeyUse && (informatin.dateKey < QDateTime::currentDateTime().toTime_t()))
                {
                    JasonQt_ShowInformationMessageBoxFromOtherThread::show("Message", QStringLiteral("程序出现内部错误\n请咨询程序开发者以解决此问题."));
                    throw QString("DateKet error, Ready exit app.");
                }
            }
            else
            {
                if(closeAppOnSelectInformationFail)
                {
                    JasonQt_ShowInformationMessageBoxFromOtherThread::show("Message", QStringLiteral("无法正确校验程序可靠性，请检查网络连接."));
                    throw QString("Select information fail, Ready exit app.");
                }
                else
                {
                    qDebug("Select information fail.");
                }
            }

            if(!append.isEmpty())
            {
                JasonQt_ShowInformationMessageBoxFromOtherThread::show("Message", append);
            }
        }
        catch(const QString message)
        {
            qDebug() << message;
            qApp->exit(-1);
            QTimer::singleShot(5000, [](){ ::exit(-2); });
            QTimer::singleShot(10000, [](){ throw JasonQt_ReadyExit("JasonLogCollectionClose"); });
        }

        Api::end();
    });
}
