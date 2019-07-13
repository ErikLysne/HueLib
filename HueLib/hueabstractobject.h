#ifndef HUEABSTRACTOBJECT_H
#define HUEABSTRACTOBJECT_H

#include <QObject>

class HueBridge;
class HueRequest;
class HueReply;
class HueSynchronizer;

class HueAbstractObject
        : public QObject
        , public std::enable_shared_from_this<HueAbstractObject>
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

    bool turnOn(const bool on = true);
    bool turnOff(const bool off = true);
    bool setHue(const int hue);
    bool setSaturation(const int saturation);
    bool setBrightness(const int brightness);
    bool setColorTemp(const int colorTemp);
    bool setXY(const double x, const double y);
    bool setAlert(const HueAlert alert);
    bool setEffect(const HueEffect effect);

    void enablePeriodicSync(const bool periodicSyncOn = true);

    virtual bool hasValidConstructor() const = 0;
    virtual bool isValid() const = 0;
    virtual int ID() const = 0;
    virtual bool synchronize() = 0;

protected:
    void setBridge(HueBridge* bridge);
    HueBridge* getBridge() const;

    static QMap<int, QJsonObject> parseJson(QJsonObject json);

    bool sendRequest(HueRequest request);
    bool sendRequest(HueRequest request, HueReply& reply);

    virtual HueRequest makePutRequest(QJsonObject json) = 0;
    virtual HueRequest makeGetRequest() = 0;

    virtual void updateOn(const bool on) = 0;
    virtual void updateHue(const int hue) = 0;
    virtual void updateSaturation(const int saturation) = 0;
    virtual void updateBrightness(const int brightness) = 0;
    virtual void updateColorTemp(const int colorTemp) = 0;
    virtual void updateXY(const double x, const double y) = 0;
    virtual void updateAlert(const HueAlert alert) = 0;
    virtual void updateEffect(const HueEffect effect) = 0;

signals:
    void synchronized();
    void valueUpdated();

private:
    HueBridge* m_bridge;
    HueSynchronizer* m_synchronizer;
    bool m_syncEnabled;

};

#endif // HUEABSTRACTOBJECT_H
