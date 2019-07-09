#ifndef HUEABSTRACTOBJECT_H
#define HUEABSTRACTOBJECT_H

#include <QObject>

class HueBridge;
class HueRequest;
class HueReply;
class HueSynchronizer;

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

    void enablePeriodicSync(bool periodicSyncOn = true);

    virtual bool synchronize() = 0;

protected:
    static QMap<int, QJsonObject> parseJson(QJsonObject json);

    bool sendRequest(HueRequest request);
    bool sendRequest(HueRequest request, HueReply& reply);

    void setBridge(HueBridge* bridge);
    HueBridge* getBridge() const;

    virtual HueRequest makePutRequest(QJsonObject json) = 0;
    virtual HueRequest makeGetRequest() = 0;

signals:
    void synchronized();

private:
    HueBridge* m_bridge;
    HueSynchronizer* m_synchronizer;
    bool m_syncEnabled;

};

#endif // HUEABSTRACTOBJECT_H
