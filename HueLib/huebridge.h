#ifndef HUEBRIDGE_H
#define HUEBRIDGE_H

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QTimer>

#include "huerequest.h"
#include "huereply.h"

// Uses the default Qt::CoarseTimer (tries to keep accuracy within 5%)
#ifndef HUE_REQUEST_TIMEOUT_MILLISECONDS
#define HUE_REQUEST_TIMEOUT_MILLISECONDS 200
#endif

/*
 * Sleep duration between consecutive API commands sent to the bridge.
 * This is to keep the number of API commands sent per second below the
 * throughput limit of the Hue Bridge
 *
 * See application note "Hue System Performance" for details
 * https://developers.meethue.com/develop/application-design-guidance/hue-system-performance/
 *
 */
#ifndef BRIDGE_SLEEP_MILLISECONDS
#define BRIDGE_SLEEP_MILLISECONDS 50
#endif


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

    HueBridge(QNetworkAccessManager* nam, QString ip, QString username);
    HueBridge(QString ip, QString username);
    ~HueBridge();

    HueReply sendRequest(const HueRequest request);
    QString getIP() const;
    QString getUsername() const;
    bool testConnection();
    bool testConnection(ConnectionStatus &status);

private:
    HueReply sendGetRequest(QString urlPath);
    HueReply sendPutRequest(QString urlPath, QJsonObject json);
    void evaluateReply(QNetworkReply* networkReply, HueReply& reply);
    void sleep(const int sleepTimeMilliseconds);

private:
    QNetworkAccessManager* m_nam;
    QString m_ip;
    QString m_username;
    QTimer* m_sleepTimer;
};

#endif // HUEBRIDGE_H
