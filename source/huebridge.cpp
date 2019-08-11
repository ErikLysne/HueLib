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

/*!
 * \class HueBridge
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief The HueBridge class represents the physical bridge connected to the network.
 *
 * The Hue bridge is the link between Hue lights and the devices and applications that interface
 * with them. To access the bridge, an application requires a valid username generated from the bridge.
 * A username can be supplied with the constructor, or one can be generated by calling the \l link() function.
 *
 * The minimum requirement to create a HueBridge object is the IP address of the bridge. In this case, the
 * HueBridge object will be constructed without a valid username, and one must be created using the \l link()
 * function. When \l link() is called, the user has 10 seconds to press the link button on the bridge. When the button
 * is pressed, a new username will be generated and stored in a file in the build folder. \l link always checks if
 * a valid username has already been created before trying to create a new one, so pressing the button must only be
 * done the first time a new application is running.
 * \code
 *  HueBridge* bridge = new HueBridge("10.0.1.14");
 *  bridge->link();
 * \endcode
 *
 * If you want to supply an existing username instead of creating a new one, this can be added as a second argument.
 *
 * \code
 *  HueBridge* bridge = new HueBridge("10.0.1.14", "1028d66426293e821ecfd9ef1a0731df");
 * \endcode
 *
 * HueBridge needs access to \e QNetworkAccessManager. By default, it will create one internally, but if this
 * resource is required elsewhere, it can be created externally and a reference to HueBridge can be provided.
 * Use a third argument to provide a pointer to a \e QNetworkAccessManager.
 * \code
 *  QNetworkAccessManager* nam = new QNetworkAccessManager();
 *  HueBridge* bridge = new HueBridge("10.0.1.14", "1028d66426293e821ecfd9ef1a0731df", nam);
 * \endcode
 *
 * \l testConnection() can be used to test the connection to the bridge.
 *
 * \code
 *  HueBridge* bridge = new HueBridge("10.0.1.14");
 *  bridge->link();
 *
 *  HueBridge::ConnectionStatus status;
 *  bridge->testConnection(status);
 *
 *  if (status == HueBridge::Success)
 *      qDebug() << "Connected to bridge.";
 * \endcode
 *
 * \sa link(), testConnection(ConnectionStatus& status)
 */

/*!
 * \enum HueBridge::ConnectionStatus
 * This enum defines the status that can be returned by \l testConnection(ConnectionStatus& status).
 * Test for various cases to provide the user with a hint about possible reason if
 * \l testConnection(ConnectionStatus& status) did not give \l HueBridge::Success.
 *
 *  \value Success
 *      Sent a command and received expected reply. Connection successful.
 *
 * \value TimedOut
 *      Tried to send a command, but did not receive a reply within the specified timeout. Most likely caused by
 *      incorrect ip address. Alternatively, timeout period can be extended by \l setNetworkRequestTimeout()
 *
 * \value JsonError
 *      Received a reply containing a JSON error. Most likely caused by an incorrect username. Use \l getLastError() to check the error.
 *
 * \value HttpError
 *      Received a reply with HTTP status different from 200. \b Note: replies with 200 status can still contain JSON errors.
 *
 * \value Unknown
 *      An unkown error has occured. Should normally never happen.
 *
 * \sa testConnection(ConnectionStatus& status), testConnection()
 *
 */


/*!
 * Constructs a HueObject with ip address \a ip.
 *
 * A pre-existing username can optionally be provided by \a username.
 *
 * HueBridge needs access to a \e QNetworkAccessManager. A pointer to a \e QNetworkAccessManager can be provided by \a nam
 * if it is constructed externally. If ommitted in the constructor, an internal \e QNetworkAccessManager will be created with
 * HueBridge as its parent.
 *
 * A \e QObject parent can be set by \a parent, otherwise the parent will be set to \e nullptr.
 *
 */
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

/*!
 * Destroys HueBridge. If a \e QNetworkAccessManager is created internally, it will be destroyed as well.
 */
HueBridge::~HueBridge()
{

}

/*!
 * \fn QString HueBridge::link(QString appName, QString deviceName)
 *
 * The first time \l link() is called in an application, the user will be prompted to press the link button on the Hue bridge.
 * The user then has 10 seconds to press the link button, after which the function will time out and return an empty \e QString.
 *
 * If the user pressed the link button before the timeout, a new username is created and returned.
 * Additionally, a hidden folder \e .huelib will be created in the build directory with a \e username.txt file containing
 * the newly created username. This also sets the username on the bridge.
 *
 * \l link() always checks if a \e username.txt file exists in the \e .huelib folder before attempting to create a new user, so calling
 * \l link() immediately after constructing HueBridge means the username never has to be set explicitly.
 *
 * \a appName and \a deviceName sets a meaningful name for the created username. Names are stored as
 * "app_name#device_name".
 *
 * \note since the security update in Hue API 1.31 released April 9. 2019, the list of registered usernames is no longer
 * accessible from the whitelist stored on the bridge. Instead, users must go to \l https://account.meethue.com/apps and register
 * their bridge to see the list of usernames.
 *
 * \code
 *  HueBridge* bridge = new HueBridge("10.0.1.14")
 *  bridge->link()
 * \endcode
 *
 * Returns an existing username if one is found, or the newly created username if one was created. Otherwise returns an empty \e QString.
 *
 */
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

        if (!firstLine.isNull()) {
            m_username = firstLine;
            return firstLine;
        }
    }

    // If not, create a new user
    QString name = appName;
    if (deviceName != "") {
        name += "#";
        name += deviceName;
    }

    QJsonObject json = {{"devicetype", name}};

    HueRequest request("", json, HueRequest::Post);
    HueReply firstReply = sendRequest(request, nullptr);

    // If wrong IP
    if (firstReply.timedOut()) {
        qDebug() << "Request timed out - verify that the IP address of the Hue bridge is correct";
        return "";
    }

    // Attempt to create user if link button has already been pressed
    QString username = createNewUser(name, firstReply);
    if (username != "")
        return username;

    // If not, check for the following errors:
    //  1   ("unauthorized user)
    //  101 ("link button not pressed")
    const int errorType = firstReply.getError().getType();

    if (errorType == 1 || errorType == 101)
        qDebug() << "Press link button now...";
    else
        return "";



    QEventLoop waitEventLoop;
    QTimer waitTimer;

    connect(&waitTimer, &QTimer::timeout, &waitEventLoop, &QEventLoop::quit);
    waitTimer.setInterval(1000);
    waitTimer.setSingleShot(false);

    const int attempts = 10;
    for (int i = 0; i < attempts; i++) {
        qDebug() << "Waiting for link button press: " << attempts-i;
        HueReply reply = sendRequest(request, nullptr);
        username = createNewUser(name, reply);
        if (username != "") {
            waitTimer.stop();
            waitEventLoop.exit();
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

/*!
 * \fn QString HueBridge::getIP() const
 *
 * Returns the IP address of the bridge.
 *
 * \sa getUsername()
 *
 */
QString HueBridge::getIP() const
{
    return m_ip;
}

/*!
 * \fn QString HueBridge::getUsername() const
 *
 * Returns the username used by the HueBridge.
 *
 * \sa getIP()
 *
 */
QString HueBridge::getUsername() const
{
    return m_username;
}

/*!
 * \fn HueReply HueBridge::getLastReply() const
 *
 * Returns the last \l HueReply received.
 *
 * \sa getLastError()
 *
 */
HueReply HueBridge::getLastReply() const
{
    return m_lastReply;
}

/*!
 * \fn HueReply HueBridge::getLastError() const
 *
 * Returns the \l HueError contained in the last \l HueReply received.
 *
 * \sa getLastReply()
 *
 */
HueError HueBridge::getLastError() const
{
    return m_lastReply.getError();
}

/*!
 * \fn bool HueBridge::testConnection(ConnectionStatus& status)
 *
 * Tests connection to the bridge. Returns \c true if the test was successful, returns \c false otherwise.
 *
 * \a status is passed by reference and set to a \l ConnectionStatus that describes the status of the test.
 *
 * \table
 * \header
 *  \li Status
 *  \li Description
 * \row
 *  \li \l HueBridge::Success
 *  \li Sent a command and received expected reply. Connection successful.
 * \row
 *  \li \l HueBridge::TimedOut
 *  \li Tried to send a command, but did not receive a reply within the specified timeout. Most likely caused by
 *      incorrect ip address. Alternatively, timeout period can be extended by \l setNetworkRequestTimeout()
 *
 * \row
 *  \li \l HueBridge::JsonError
 *  \li Received a reply containing a JSON error. Most likely caused by an incorrect username. Use \l getLastError() to check the error.
 *
 * \row
 *  \li \l HueBridge::HttpError
 *  \li Received a reply with HTTP status different from 200. \b Note: replies with 200 status can still contain JSON errors.
 *
 * \row
 *  \li \l HueBridge::Unknown
 *  \li An unkown error has occured. Should normally never happen.
 * \endtable
 *
 * \sa testConnection()
 *
 */
bool HueBridge::testConnection(ConnectionStatus& status)
{   
    HueRequest request("lights", QJsonObject(), HueRequest::Get);
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

/*!
 * \fn bool HueBridge::testConnection()
 *
 * Tests connection to the bridge. Returns \c true if the test was successful, returns \c false otherwise.
 *
 * \sa testConnection(ConnectionStatus& status)
 *
 */
bool HueBridge::testConnection()
{
    ConnectionStatus status = ConnectionStatus::Success;
    return testConnection(status);
}

/*!
 * \fn HueReply HueBridge::sendRequest(const HueRequest request, HueAbstractObject* senderObject)
 *
 * Sends \a request to the bridge with a pointer to the sending object specified by \a senderObject.
 *
 * \note This function is not meant to be called explicitly; it is used by objects derived from
 * \l HueAbstractObject to send network requests to the bridge.
 *
 * Returns \l HueReply.
 *
 */
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
    case HueRequest::Get:
    case HueRequest::Put:
        url = "http://" + m_ip + "/api/" + m_username + "/" + urlPath;
        break;
    case HueRequest::Post:
        url = "http://" + m_ip + "/api";
        break;
    }

    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(url));

    if (method == HueRequest::Post)
        networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));

    QJsonDocument jsonDoc(json);
    QByteArray jsonBytes = jsonDoc.toJson();

    QEventLoop eventLoop;
    QTimer eventTimer;
    connect(&eventTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);

    QNetworkReply* networkReply;
    switch (method) {
    case HueRequest::Get:
        networkReply = m_nam->get(networkRequest);
        break;
    case HueRequest::Put:
        networkReply = m_nam->put(networkRequest, jsonBytes);
        break;
    case HueRequest::Post:
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
    // Command sent from some other object - use the bridge command block time
    else
        m_blockTimer->start(m_bridgeCommandBlockTime);


    networkReply->deleteLater();
    return reply;
}

/*!
 * \fn void HueBridge::setNetworkAccessManager(QNetworkAccessManager* nam)
 *
 * Sets the \e QNetworkAccessManager to \a nam.
 *
 */
void HueBridge::setNetworkAccessManager(QNetworkAccessManager* nam)
{
    if (nam == nullptr)
        return;

    if (m_nam != nullptr)
        delete m_nam;

    m_nam = nam;
}

/*!
 * \fn void HueBridge::setLightCommandBlockTime(const int milliseconds)
 *
 * Sets the time (in milliseconds) the bridge is blocked for after \l sendRequest() has
 * been called by a \l HueLight object. This is set to limit traffic to the
 * throughput of the bridge. Setting a low value can lead to higher throughput, but
 * can lead to more frequent timeouts and potentially make the bridge less responsive.
 *
 * Block time is specified by \a milliseconds.
 *
 * \sa setGroupCommandBlockTime(), setBridgeCommandBlockTime(), setNetworkRequestTimeout()
 *
 */
void HueBridge::setLightCommandBlockTime(const int milliseconds)
{
    if (milliseconds > 0)
        m_lightCommandBlockTime = milliseconds;
    else
        m_lightCommandBlockTime = m_defaultLightCommandBlockTime;
}

/*!
 * \fn void HueBridge::setGroupCommandBlockTime(const int milliseconds)
 *
 * Sets the time (in milliseconds) the bridge is blocked for after \l sendRequest() has
 * been called by a \l HueGroup object. This is set to limit traffic to the
 * throughput of the bridge. Setting a low value can lead to higher throughput, but
 * can lead to more frequent timeouts and potentially make the bridge less responsive.
 *
 * Block time is specified by \a milliseconds.
 *
 * \sa setLightCommandBlockTime(), setBridgeCommandBlockTime(), setNetworkRequestTimeout()
 *
 */
void HueBridge::setGroupCommandBlockTime(const int milliseconds)
{
    if (milliseconds > 0)
        m_groupCommandBlockTime = milliseconds;
    else
        m_groupCommandBlockTime = m_defaultGroupCommandBlockTime;
}

/*!
 * \fn void HueBridge::setBridgeCommandBlockTime(const int milliseconds)
 *
 * Sets the time (in milliseconds) the bridge is blocked for after \l sendRequest() has
 * been called without a specified object, such as when calling \e discover functions.
 * This is set to limit traffic to the throughput of the bridge. Setting a low value can
 * lead to higher throughput, but can lead to more frequent timeouts and potentially make
 * the bridge less responsive.
 *
 * Block time is specified by \a milliseconds.
 *
 * \sa setLightCommandBlockTime(), setGroupCommandBlockTime(), setNetworkRequestTimeout()
 *
 */
void HueBridge::setBridgeCommandBlockTime(const int milliseconds)
{
    if (milliseconds > 0)
        m_bridgeCommandBlockTime = milliseconds;
    else
        m_bridgeCommandBlockTime = m_defaultBridgeCommandBlockTime;
}

/*!
 * \fn void HueBridge::setNetworkRequestTimeout(const int milliseconds)
 *
 * Sets the time (in milliseconds) until \e timeout after calling \l sendRequest().
 * \note Reducing the timeout period for network requests will not make the bridge more responsive.
 *
 * Timeout period is specified by \a milliseconds.
 *
 * \sa setLightCommandBlockTime(), setGroupCommandBlockTime(), setBridgeCommandBlockTime()
 *
 */
void HueBridge::setNetworkRequestTimeout(const int milliseconds)
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

    m_username = username;
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
