#include "JasonQt_Vop.h"

using namespace JasonQt_Vop;

// BaiduTokenFoundation
BaiduTokenFoundation::BaiduTokenFoundation(const QString &apiKey, const QString &secretKey):
    m_apiKey(apiKey),
    m_secretKey(secretKey)
{ }

bool BaiduTokenFoundation::refreshToken(const bool &onNewThread)
{
    if(onNewThread)
    {
        QtConcurrent::run([=]()
        {
            this->refreshToken(false);
        });
        return true;
    }
    else
    {
        QNetworkRequest request(QUrl(QString("https://openapi.baidu.com/oauth/2.0/token?")));
        QByteArray append = QString("grant_type=client_credentials&client_id=%1&client_secret=%2&").
                            arg(m_apiKey, m_secretKey).toLatin1();
        QByteArray buf;

        request.setRawHeader("Content-Type", "application/json");

        bool flag;
        if(m_http.thread() != QThread::currentThread())
        {
            flag = JasonQt_Net::HTTP().post(request, append, buf, 15000);
        }
        else
        {
            flag = m_http.post(request, append, buf, 15000);
        }
        if(!flag) { return false; }

        const auto &&data = QJsonDocument::fromJson(buf).object();
        if(data.isEmpty() || !data.contains("access_token")) { return false; }

        m_token = data["access_token"].toString();
        return true;
    }
}

// BaiduVop
BaiduVop::BaiduVop(const QString &apiKey, const QString &secretKey):
    BaiduTokenFoundation(apiKey, secretKey)
{
    const auto &&availableDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    if(!availableDevices.isEmpty())
    {
        this->setDevice(availableDevices.first());
    }

    static bool flag = true;
    if(flag)
    {
        flag = false;
        qRegisterMetaType<QAudio::State>("QAudio::State");
    }
}

BaiduVop::~BaiduVop(void)
{
    if(m_audioInput)
    {
        m_audioInput->deleteLater();
    }
}

void BaiduVop::setDevice(const QAudioDeviceInfo &device)
{
    if(m_audioInput)
    {
        m_audioInput->deleteLater();
    }

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");

    m_audioInput = new QAudioInput(device, format, this);
}

bool BaiduVop::start(void)
{
    if(m_token.isEmpty())
    {
        qDebug("BaiduVop::start fail, Need refresh token befor start.");
        return false;
    }
    if(!m_audioInput)
    {
        qDebug("BaiduVop::start fail, Audio input is NULL.");
        return false;
    }

    m_buffer = new QBuffer;
    m_buffer->open(QIODevice::ReadWrite);
    m_audioInput->start(m_buffer);

    return true;
}

std::pair<bool, QString> BaiduVop::finish(void)
{
    if(!m_audioInput)
    {
        qDebug("BaiduVop::finish fail, Audio input is NULL.");
        return { false, QString() };
    }
    if(!m_buffer)
    {
        qDebug("BaiduVop::finish fail, Buffer is NULL.");
        return { false, QString() };
    }

    m_audioInput->stop();

    const auto &sendData = m_buffer->data();
    m_buffer->deleteLater();

    QNetworkRequest request(QUrl("http://vop.baidu.com/server_api"));
    QJsonObject append;

    request.setRawHeader("Content-Type", "application/json");

    append["format"] = "pcm";
    append["rate"] = 8000;
    append["channel"] = 1;
    append["token"] = m_token;
    append["lan"] = "zh";
    append["cuid"] = "JasonQt";
    append["speech"] = QString(sendData.toBase64());
    append["len"] = sendData.size();

    QByteArray buf;
    m_http.post(request, QJsonDocument(append).toJson(), buf, 15000);

    QJsonObject acceptedData(QJsonDocument::fromJson(buf).object());

    if(buf.isEmpty() || acceptedData.isEmpty() || !acceptedData.contains("result")) { return { false, buf }; }

    const auto &&message = acceptedData["result"].toArray()[0].toString();
    return { true, message.mid(0, message.size() - 1) };
}

// BaiduText2Audio
BaiduText2Audio::BaiduText2Audio(const QString &apiKey, const QString &secretKey):
    BaiduTokenFoundation(apiKey, secretKey)
{
    const auto &&availableDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    if(!availableDevices.isEmpty())
    {
        this->setDevice(availableDevices.first());
    }
}

BaiduText2Audio::~BaiduText2Audio(void)
{
    if(m_audioOutput)
    {
        m_audioOutput->deleteLater();
    }
}

void BaiduText2Audio::setDevice(const QAudioDeviceInfo &device)
{
    if(m_audioOutput)
    {
        m_audioOutput->deleteLater();
    }

    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");

    m_audioOutput = new QAudioOutput(device, format, this);
}

bool BaiduText2Audio::play(const QString &message)
{
    QNetworkRequest request(QUrl("http://tsn.baidu.com/text2audio"));
    QString format("tex=%1&lan=%2&cuid=%3&ctp=%4&tok=%5");

    request.setRawHeader("Content-Type", "application/json");

    const auto &&data = format.arg(message, "zh", "JasonQt", "1", m_token).toUtf8();

    QByteArray buf;
    if(!m_http.post(request, data, buf, 15000))
    {
        return false;
    }

    if(m_buffer)
    {
        m_audioOutput->stop();
        m_buffer->deleteLater();
    }

    m_buffer = new QBuffer;
    m_buffer->setData(buf);
    m_buffer->open(QIODevice::ReadOnly);

    qDebug() << buf;
    qDebug() << buf.size();

    m_audioOutput->start(m_buffer);

//    QFile file("/Users/Jason/test.wav");
//    file.open(QIODevice::WriteOnly);
//    file.write(buf);

//    QFile *file2 = new QFile("/Users/Jason/test.wav");
//    file2->open(QIODevice::ReadOnly);
//    m_audioOutput->start(file2);

//    QSound::play("/Users/Jason/test.wav");

    return true;
}
