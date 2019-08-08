#ifndef HUEBRIDGE_H
#define HUEBRIDGE_H

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QTimer>

#include "huereply.h"

class HueRequest;
class HueError;

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

    QString createNewUser(QString app = "C++ HueLib app", QString device = "");
    bool testConnection(ConnectionStatus &status);
    bool testConnection();

    QString getIP() const;
    QString getUsername() const;
    HueReply getLastReply() const;
    HueError getLastError() const;

    HueReply sendRequest(const HueRequest request);
    void setNetworkRequestTimeout(int timeoutMilliseconds);

private:
    void evaluateReply(QNetworkReply* networkReply, HueReply& reply);
    void sleep(const int sleepTimeMilliseconds);

private:
    const int m_sleepTimeMilliSec = 50;
    const int m_requestTimeoutMilliSec = 200;

    QNetworkAccessManager* m_nam;
    QString m_ip;
    QString m_username;
    HueReply m_lastReply;
    QTimer* m_sleepTimer;
    int m_networkTimeoutMilliSec;

};

#endif // HUEBRIDGE_H
