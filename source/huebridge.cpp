#include "huebridge.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QEventLoop>

#include "huerequest.h"
#include "hueerror.h"

HueBridge::HueBridge(QString ip, QString username, QNetworkAccessManager* nam, QObject* parent)
    : QObject(parent)
    , m_nam(nam)
    , m_ip(ip)
    , m_username(username)
    , m_lastReply()
    , m_sleepTimer(new QTimer(this))
    , m_networkTimeoutMilliSec(m_sleepTimeMilliSec)
{
    m_nam->setParent(this);
    m_sleepTimer->setSingleShot(true);
}

HueBridge::~HueBridge()
{

}

QString HueBridge::createNewUser(QString app, QString device)
{
    QString userID = app;
    if (device != "") {
        userID += "#";
        userID += device;
    }

    QJsonObject json = {{"devicetype", userID}};

    HueRequest request("", json, HueRequest::post);
    HueReply firstReply = sendRequest(request);

    // If wrong IP
    if (firstReply.timedOut()) {
        qDebug() << "Request timed out - verify that the IP address of the Hue bridge is correct";
        return "";
    }

    // If link button is already pressed
    if (!firstReply.containsError() && firstReply.getJson().contains("username")) {
        QString username = firstReply.getJson()["username"].toString();
        qDebug() << "New user created: " << username;
        return username;
    }

    // If not, ask user to press it
    if (firstReply.getError().getType() == 101)
        qDebug() << "Press link button now...";

    QEventLoop waitEventLoop;
    QTimer waitTimer;

    connect(&waitTimer, &QTimer::timeout, &waitEventLoop, &QEventLoop::quit);
    waitTimer.setInterval(1000);
    waitTimer.setSingleShot(false);

    int attempts = 10;
    for (int i = 0; i < attempts; i++) {
        qDebug() << "Attempting to create new user: " << attempts-i;
        HueReply nextReply = sendRequest(request);
        if (!nextReply.containsError() && nextReply.getJson().contains("username")) {
            QString username = nextReply.getJson()["username"].toString();
            qDebug() << "New user created: " << username;
            return username;
        }
        waitTimer.start();
        waitEventLoop.exec();
    }

    qDebug() << "Link button was not pressed in time.";

    return "";
}

QString HueBridge::getIP() const
{
    return m_ip;
}

QString HueBridge::getUsername() const
{
    return m_username;
}

HueReply HueBridge::getLastReply() const
{
    return m_lastReply;
}

HueError HueBridge::getLastError() const
{
    return m_lastReply.getError();
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
    else if (reply.getError().getType() != 0) {
        status = ConnectionStatus::JsonError;
    }
    else {
        status = ConnectionStatus::Unknown;
    }

    return false;
}

bool HueBridge::testConnection()
{
    ConnectionStatus status = ConnectionStatus::Success;
    return testConnection(status);
}

HueReply HueBridge::sendRequest(const HueRequest request)
{
    if (m_sleepTimer->isActive()) {
        int remainingTime = m_sleepTimer->remainingTime();
        m_sleepTimer->stop();
        sleep(remainingTime);
    }

    QString urlPath = request.getUrlPath();
    QJsonObject json = request.getJson();
    HueRequest::Method method = request.getMethod();

    HueReply reply;
    reply.timedOut(false);
    reply.isValid(true);

    QString url;

    switch (method) {
    case HueRequest::get:
    case HueRequest::put:
        url = "http://" + m_ip + "/api/" + m_username + "/" + urlPath;
        break;
    case HueRequest::post:
        url = "http://" + m_ip + "/api";
        break;
    }

    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(url));

    if (method == HueRequest::post)
        networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));

    QJsonDocument jsonDoc(json);
    QByteArray jsonBytes = jsonDoc.toJson();

    QEventLoop eventLoop;
    QTimer eventTimer;
    connect(&eventTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);

    QNetworkReply* networkReply;
    switch (method) {
    case HueRequest::get:
        networkReply = m_nam->get(networkRequest);
        break;
    case HueRequest::put:
        networkReply = m_nam->put(networkRequest, jsonBytes);
        break;
    case HueRequest::post:
        networkReply = m_nam->post(networkRequest, jsonBytes);
        break;
    }

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

void HueBridge::setNetworkRequestTimeout(int timeoutMilliseconds)
{
    if (timeoutMilliseconds > 0)
        m_networkTimeoutMilliSec = timeoutMilliseconds;
    else
        m_networkTimeoutMilliSec = m_requestTimeoutMilliSec;
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

            HueError error;
            error.setType(jsonError["type"].toInt());
            error.setAddress(jsonError["address"].toString());
            error.setDescription(jsonError["description"].toString());

            reply.setError(error);
            reply.isValid(false);
            reply.setJson(jsonError);
        }
        else if (jsonRootObject.contains("success")) {
            QJsonObject jsonSuccess = jsonRootObject["success"].toObject();
            reply.isValid(true);
            reply.setJson(jsonSuccess);
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

    m_lastReply = reply;
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
