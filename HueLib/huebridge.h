#ifndef HUEBRIDGE_H
#define HUEBRIDGE_H

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QTimer>

class HueRequest;
class HueReply;

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

    HueBridge(QString ip, QString username,
              QNetworkAccessManager* nam = new QNetworkAccessManager(),
              QObject* parent = nullptr);
    ~HueBridge();

    HueReply sendRequest(const HueRequest request);
    QString getIP() const;
    QString getUsername() const;
    bool testConnection();
    bool testConnection(ConnectionStatus &status);
    void setNetworkRequestTimeout(int timeoutMilliseconds);

private:
    HueReply sendGetRequest(QString urlPath);
    HueReply sendPutRequest(QString urlPath, QJsonObject json);
    void evaluateReply(QNetworkReply* networkReply, HueReply& reply);
    void sleep(const int sleepTimeMilliseconds);

private:
    const int m_sleepTimeMilliSec = 50;
    const int m_requestTimeoutMilliSec = 200;

    QNetworkAccessManager* m_nam;
    QString m_ip;
    QString m_username;
    QTimer* m_sleepTimer;
    int m_networkTimeoutMilliSec;

};

#endif // HUEBRIDGE_H
