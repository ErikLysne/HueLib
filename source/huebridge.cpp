#include "huebridge.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QEventLoop>
#include <QDir>

#include "huerequest.h"
#include "hueerror.h"
#include "huelight.h"
#include "huegroup.h"

HueBridge::HueBridge(QString ip, QString username, QNetworkAccessManager* nam, QObject* parent)
    : QObject(parent)
    , m_nam(nam)
    , m_ip(ip)
    , m_username(username)
    , m_lastReply()
    , m_blockTimer(new QTimer(this))
    , m_lightCommandBlockTime(m_defaultLightCommandBlockTime)
    , m_groupCommandBlockTime(m_defaultGroupCommandBlockTime)
    , m_bridgeCommandBlockTime(m_defaultBridgeCommandBlockTime)
    , m_networkRequestTimeout(m_defaultNetworkRequestTimeout)
{
    m_nam->setParent(this);
    m_blockTimer->setSingleShot(true);
}

HueBridge::~HueBridge()
{

}

QString HueBridge::link(QString appName, QString deviceName)
{
    // First check if a user has already been created for this application
    QDir hueDir(".hue");
    if (!hueDir.exists())
        hueDir.mkpath(".");

    QFile hueUserFile(".hue/username.txt");
    if (hueUserFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&hueUserFile);
        QString firstLine = stream.readLine();
        hueUserFile.close();

        if (!firstLine.isNull())
            return firstLine;
    }

    // If not, create a new user
    QString name = appName;
    if (deviceName != "") {
        name += "#";
        name += deviceName;
    }

    QJsonObject json = {{"devicetype", name}};

    HueRequest request("", json, HueRequest::post);
    HueReply firstReply = sendRequest(request, nullptr);

    // If wrong IP
    if (firstReply.timedOut()) {
        qDebug() << "Request timed out - verify that the IP address of the Hue bridge is correct";
        return "";
    }

    // Attempt to create user if link button has already been pressed
    QString username = createNewUser(name, firstReply);
    if (username != "") {
        m_username = username;
        return username;
    }

    // If not, check for 101 error ("link button not pressed")
    if (firstReply.getError().getType() == 101)
        qDebug() << "Press link button now...";
    else
        return "";

    QEventLoop waitEventLoop;
    QTimer waitTimer;

    connect(&waitTimer, &QTimer::timeout, &waitEventLoop, &QEventLoop::quit);
    waitTimer.setInterval(1000);
    waitTimer.setSingleShot(false);

    int attempts = 10;
    for (int i = 0; i < attempts; i++) {
        qDebug() << "Waiting for link button press: " << attempts-i;
        HueReply reply = sendRequest(request, nullptr);
        username = createNewUser(name, reply);
        if (username != "") {
            waitTimer.stop();
            waitEventLoop.exit();
            m_username = username;
            return username;
        }
        waitTimer.start();
        waitEventLoop.exec();
    }

    qDebug() << "Link button was not pressed within the time limit. Please try again.";

    waitTimer.stop();
    waitEventLoop.exit();
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
    HueReply reply = sendRequest(request, nullptr);

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

HueReply HueBridge::sendRequest(const HueRequest request, HueAbstractObject* senderObject)
{
    if (m_blockTimer->isActive()) {
        int remainingTime = m_blockTimer->remainingTime();
        m_blockTimer->stop();
        block(remainingTime);
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
    },
    Qt::DirectConnection);
    connect(networkReply, &QNetworkReply::finished,  &eventLoop, &QEventLoop::quit, Qt::DirectConnection);

    eventTimer.setSingleShot(true);
    eventTimer.start(m_networkRequestTimeout);

    eventLoop.exec();

    if (eventTimer.isActive()) {
        eventTimer.stop();
    }
    else {
        networkReply->abort();
        reply.timedOut(true);
        reply.isValid(false);
    }

    // Block further bridge calls based on the type of command being sent
    // HueGroup commands have lower throughput than HueLight commands

    // General bridge/discovery command
    if (senderObject == nullptr)
        m_blockTimer->start(m_bridgeCommandBlockTime);
    // Command sent from HueLight object
    else if (dynamic_cast<HueLight*>(senderObject) != NULL)
        m_blockTimer->start(m_lightCommandBlockTime);
    // Command sent from HueGroup Object
    else if (dynamic_cast<HueGroup*>(senderObject) != NULL)
        m_blockTimer->start(m_groupCommandBlockTime);


    networkReply->deleteLater();
    return reply;
}

void HueBridge::setLightCommandBlockTime(int milliseconds)
{
    if (milliseconds > 0)
        m_lightCommandBlockTime = milliseconds;
    else
        m_lightCommandBlockTime = m_defaultLightCommandBlockTime;
}

void HueBridge::setGroupCommandBlockTime(int milliseconds)
{
    if (milliseconds > 0)
        m_groupCommandBlockTime = milliseconds;
    else
        m_groupCommandBlockTime = m_defaultGroupCommandBlockTime;
}

void HueBridge::setBridgeCommandBlockTime(int milliseconds)
{
    if (milliseconds > 0)
        m_bridgeCommandBlockTime = milliseconds;
    else
        m_bridgeCommandBlockTime = m_defaultBridgeCommandBlockTime;
}

void HueBridge::setNetworkRequestTimeout(int milliseconds)
{
    if (milliseconds > 0)
        m_networkRequestTimeout = milliseconds;
    else
        m_networkRequestTimeout = m_defaultNetworkRequestTimeout;
}

QString HueBridge::createNewUser(QString name, HueReply reply)
{
    if (reply.containsError() || !reply.getJson().contains("username"))
        return "";

    QString username = reply.getJson()["username"].toString();
    qDebug() << "New user created:\n" << name << ": " << username;

    QFile hueUserFile(".hue/username.txt");
    if (hueUserFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&hueUserFile);
        stream << username << "\n\r";
        stream << "Application Name: " << name;
        hueUserFile.close();
    }

    return username;
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

void HueBridge::block(const int sleepTimeMilliseconds)
{
    QEventLoop blockEventLoop;
    QTimer blockTimer;

    connect(&blockTimer, &QTimer::timeout, &blockEventLoop, &QEventLoop::quit);

    blockTimer.setSingleShot(true);
    blockTimer.start(sleepTimeMilliseconds);

    blockEventLoop.exec();
}
