#include "huebridge.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QEventLoop>

#include "huerequest.h"
#include "huereply.h"

HueBridge::HueBridge(QString ip, QString username, QNetworkAccessManager* nam, QObject* parent)
    : QObject(parent)
    , m_nam(nam)
    , m_ip(ip)
    , m_username(username)
    , m_sleepTimer(new QTimer(this))
    , m_networkTimeoutMilliSec(m_sleepTimeMilliSec)
{
    m_nam->setParent(this);
    m_sleepTimer->setSingleShot(true);
}

HueBridge::~HueBridge()
{

}

HueReply HueBridge::sendRequest(const HueRequest request)
{
    if (m_sleepTimer->isActive()) {
        int remainingTime = m_sleepTimer->remainingTime();
        m_sleepTimer->stop();
        sleep(remainingTime);
    }

    QString url = request.getUrlPath();
    QJsonObject json = request.getJson();
    HueRequest::Method method = request.getMethod();

    switch (method) {
    case HueRequest::get:
        return sendGetRequest(url);
    case HueRequest::put:
        return sendPutRequest(url, json);
    }
}

QString HueBridge::getIP() const
{
    return m_ip;
}

QString HueBridge::getUsername() const
{
    return m_username;
}

bool HueBridge::testConnection(ConnectionStatus &status)
{
    HueRequest request("lights", QJsonObject(), HueRequest::get);
    HueReply reply = sendRequest(request);

    if (reply.isValid()) {
        status = ConnectionStatus::Success;
        return true;
    }
    else if (reply.timedOut()) {
        status = ConnectionStatus::TimedOut;
    }
    else if (reply.getHttpStatus() != 200) {
        status = ConnectionStatus::HttpError;
    }
    else if (reply.getErrorType() != 0) {
        status = ConnectionStatus::JsonError;
    }
    else {
        status = ConnectionStatus::Unknown;
    }

    return false;
}

bool HueBridge::testConnection()
{
    ConnectionStatus status = Success;
    return testConnection(status);
}

void HueBridge::setNetworkRequestTimeout(int timeoutMilliseconds)
{
    if (timeoutMilliseconds > 0)
        m_networkTimeoutMilliSec = timeoutMilliseconds;
    else
        m_networkTimeoutMilliSec = m_requestTimeoutMilliSec;
}

HueReply HueBridge::sendGetRequest(QString urlPath)
{
    HueReply reply;
    reply.timedOut(false);
    reply.isValid(true);

    QNetworkRequest request;
    QString url = "http://" + m_ip + "/api/" + m_username + "/" + urlPath;
    request.setUrl(QUrl(url));

    QEventLoop eventLoop;
    QTimer eventTimer;
    connect(&eventTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);

    QNetworkReply* networkReply = m_nam->get(request);
    connect(networkReply, &QNetworkReply::finished,
            this, [this, &reply]()
    {
        QNetworkReply* networkReply = qobject_cast<QNetworkReply*>(sender());
        evaluateReply(networkReply, reply);
    });

    connect(networkReply, &QNetworkReply::finished,  &eventLoop, &QEventLoop::quit);

    eventTimer.setSingleShot(true);
    eventTimer.start(m_networkTimeoutMilliSec);

    eventLoop.exec();

    if (eventTimer.isActive()) {
        eventTimer.stop();
    }
    else {
        networkReply->abort();
        reply.timedOut(true);
        reply.isValid(false);
    }

    networkReply->deleteLater();
    m_sleepTimer->start(m_sleepTimeMilliSec);
    return reply;
}

HueReply HueBridge::sendPutRequest(QString urlPath, QJsonObject json)
{
    HueReply reply;
    reply.timedOut(false);
    reply.isValid(true);

    QNetworkRequest request;
    QString url = "http://" + m_ip + "/api/" + m_username + "/" + urlPath;
    request.setUrl(QUrl(url));

    QJsonDocument jsonDoc(json);
    QByteArray jsonBytes = jsonDoc.toJson();

    QEventLoop eventLoop;
    QTimer eventTimer;
    connect(&eventTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);

    QNetworkReply* networkReply = m_nam->put(request, jsonBytes);
    connect(networkReply, &QNetworkReply::finished,
            this, [this, &reply]()
    {
        QNetworkReply* networkReply = qobject_cast<QNetworkReply*>(sender());
        evaluateReply(networkReply, reply);
    });
    connect(networkReply, &QNetworkReply::finished,  &eventLoop, &QEventLoop::quit);

    eventTimer.setSingleShot(true);
    eventTimer.start(m_networkTimeoutMilliSec);

    eventLoop.exec();

    if (eventTimer.isActive()) {
        eventTimer.stop();
    }
    else {
        networkReply->abort();
        reply.timedOut(true);
        reply.isValid(false);
    }

    m_sleepTimer->start(m_sleepTimeMilliSec);
    networkReply->deleteLater();
    return reply;
}


void HueBridge::evaluateReply(QNetworkReply* networkReply, HueReply& reply)
{
    int statusCode = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply.setHttpStatus(statusCode);

    QByteArray replyBytes = networkReply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(replyBytes);

    if (jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();
        QJsonObject jsonRootObject = jsonArray[0].toObject();
        if (jsonRootObject.contains("error")) {
            QJsonObject jsonError = jsonRootObject["error"].toObject();

            int errorType               = jsonError["type"].toInt();
            QString errorAddress        = jsonError["address"].toString();
            QString errorDescription    = jsonError["description"].toString();

            reply.setErrorType(errorType);
            reply.setErrorAddress(errorAddress);
            reply.setErrorDescription(errorDescription);
            reply.isValid(false);
        }
        else if (jsonRootObject.contains("success")) {
            reply.isValid(true);
        }
    }
    else if (!jsonDoc.isEmpty()){
        QJsonObject jsonContent = jsonDoc.object();
        reply.setJson(jsonContent);
        reply.isValid(true);
    }
    else {
        reply.isValid(false);
    }
}

void HueBridge::sleep(const int sleepTimeMilliseconds)
{
    QEventLoop sleepEventLoop;
    QTimer sleepTimer;

    connect(&sleepTimer, &QTimer::timeout, &sleepEventLoop, &QEventLoop::quit);

    sleepTimer.setSingleShot(true);
    sleepTimer.start(sleepTimeMilliseconds);

    sleepEventLoop.exec();
}
