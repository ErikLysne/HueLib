#ifndef HUEABSTRACTOBJECT_H
#define HUEABSTRACTOBJECT_H

#include <QObject>
#include "huebridge.h"

class HueAbstractObject : public QObject
{
    Q_OBJECT
public:
    enum HueAlert {
        NoAlert,
        BreatheSingle,
        Breathe15Sec
    };
    enum HueEffect {
        NoEffect,
        ColorLoop
    };
    explicit HueAbstractObject(HueBridge* bridge);
    virtual ~HueAbstractObject() {}

    bool turnOn();
    bool turnOff();
    bool setHue(int hue);
    bool setSaturation(int saturation);
    bool setBrightness(int brightness);
    bool setColorTemp(int colorTemp);
    bool setXY(double x, double y);
    bool setAlert(HueAlert alert);
    bool setEffect(HueEffect effect);

    void synchronizePeriodically(bool enable = true);
    void setSynchronizationInterval(int intervalSeconds);

    virtual bool synchronize() = 0;

protected:
    static QMap<int, QJsonObject> parseJson(QJsonObject json);

    bool sendRequest(HueRequest request);
    bool sendRequest(HueRequest request, HueReply& reply);
    void setBridge(HueBridge* bridge);
    HueBridge* getBridge() const;

    virtual HueRequest makePutRequest(QJsonObject json) = 0;
    virtual HueRequest makeGetRequest() = 0;

protected slots:
    void timeToSynchronize();

private:
    HueBridge* m_bridge;
    int m_syncIntervalSeconds;
    static QTimer* m_syncTimer;
};

#endif // HUEABSTRACTOBJECT_H
