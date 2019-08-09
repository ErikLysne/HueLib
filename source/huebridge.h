#ifndef HUEBRIDGE_H
#define HUEBRIDGE_H

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QTimer>

#include "huereply.h"

class HueRequest;
class HueError;
class HueAbstractObject;

class HueBridge : public QObject
{
    Q_OBJECT
public:
    enum ConnectionStatus {
        Success,
        TimedOut,
        JsonError,
        HttpError,
        Unknown
    };

    HueBridge(QString ip, QString username = "",
              QNetworkAccessManager* nam = new QNetworkAccessManager(),
              QObject* parent = nullptr);
    ~HueBridge();

    QString link(QString appName = "C++ HueLib app", QString deviceName = "");
    bool testConnection(ConnectionStatus &status);
    bool testConnection();

    QString getIP() const;
    QString getUsername() const;
    HueReply getLastReply() const;
    HueError getLastError() const;

    HueReply sendRequest(const HueRequest request, HueAbstractObject* senderObject);

    void setNetworkAccessManager(QNetworkAccessManager* nam);
    void setLightCommandBlockTime(const int milliseconds);
    void setGroupCommandBlockTime(const int milliseconds);
    void setBridgeCommandBlockTime(const int milliseconds);
    void setNetworkRequestTimeout(const int milliseconds);

private:
    QString createNewUser(QString name, HueReply reply);
    void evaluateReply(QNetworkReply* networkReply, HueReply& reply);
    void block(const int sleepTimeMilliseconds);

private:
    const int m_defaultLightCommandBlockTime = 50;
    const int m_defaultGroupCommandBlockTime = 100;
    const int m_defaultBridgeCommandBlockTime = 200;
    const int m_defaultNetworkRequestTimeout = 200;

    QNetworkAccessManager* m_nam;
    QString m_ip;
    QString m_username;
    HueReply m_lastReply;
    QTimer* m_blockTimer;

    int m_lightCommandBlockTime;
    int m_groupCommandBlockTime;
    int m_bridgeCommandBlockTime;
    int m_networkRequestTimeout;

};

#endif // HUEBRIDGE_H
