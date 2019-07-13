#include "hueabstractobject.h"

#include <QJsonArray>

#include "huebridge.h"
#include "huerequest.h"
#include "huereply.h"
#include "huesynchronizer.h"

HueAbstractObject::HueAbstractObject(HueBridge* bridge) :
    QObject(nullptr),
    m_bridge(bridge),
    m_syncEnabled(false)
{

}

bool HueAbstractObject::turnOn(const bool on)
{
    QJsonObject json {
        {"on", on}
    };

    HueRequest request = makePutRequest(json);
    bool updateSuccessful = sendRequest(request);

    if (updateSuccessful)
        updateOn(on);

    return updateSuccessful;
}

bool HueAbstractObject::turnOff(const bool off)
{
    QJsonObject json {
        {"on", !off}
    };

    HueRequest request = makePutRequest(json);
    bool updateSuccessful = sendRequest(request);

    if (updateSuccessful)
        updateOn(!off);

    return updateSuccessful;
}

bool HueAbstractObject::setHue(const int hue)
{
    QJsonObject json {
        {"hue", hue}
    };

    HueRequest request = makePutRequest(json);
    bool updateSuccessful = sendRequest(request);

    if (updateSuccessful)
        updateHue(hue);

    return updateSuccessful;
}

bool HueAbstractObject::setSaturation(const int saturation)
{
    QJsonObject json {
        {"sat", saturation}
    };

    HueRequest request = makePutRequest(json);
    bool updateSuccessful = sendRequest(request);

    if (updateSuccessful)
        updateSaturation(saturation);

    return updateSuccessful;
}

bool HueAbstractObject::setBrightness(const int brightness)
{
    QJsonObject json {
        {"bri", brightness}
    };

    HueRequest request = makePutRequest(json);
    bool updateSuccessful = sendRequest(request);

    if (updateSuccessful)
        updateBrightness(brightness);

    return updateSuccessful;
}

bool HueAbstractObject::setColorTemp(const int colorTemp)
{
    QJsonObject json {
        {"ct", colorTemp}
    };

    HueRequest request = makePutRequest(json);
    bool updateSuccessful = sendRequest(request);

    if (updateSuccessful)
        updateColorTemp(colorTemp);

    return updateSuccessful;
}

bool HueAbstractObject::setXY(const double x, const double y)
{
    QJsonArray xy = {x, y};
    QJsonObject json {
        {"ct", xy}
    };

    HueRequest request = makePutRequest(json);
    bool updateSuccessful = sendRequest(request);

    if (updateSuccessful)
        updateXY(x, y);

    return updateSuccessful;
}

bool HueAbstractObject::setAlert(const HueAlert alert)
{
    QJsonObject json;
    switch (alert) {
    case NoAlert:
        json = {
            {"alert", "none"}
        };
        break;
    case BreatheSingle:
        json = {
            {"alert", "select"}
        };
        break;
    case Breathe15Sec:
        json = {
            {"alert", "lselect"}
        };
        break;
    }

    HueRequest request = makePutRequest(json);
    bool updateSuccessful = sendRequest(request);

    if (updateSuccessful)
        updateAlert(alert);

    return updateSuccessful;
}

bool HueAbstractObject::setEffect(const HueEffect effect)
{
    QJsonObject json;
    switch (effect) {
    case NoEffect:
        json = {
            {"effect", "none"}
        };
        break;
    case ColorLoop:
        json = {
            {"effect", "colorloop"}
        };
        break;
    }

    HueRequest request = makePutRequest(json);
    bool updateSuccessful = sendRequest(request);

    if (updateSuccessful)
        updateEffect(effect);

    return updateSuccessful;
}

void HueAbstractObject::enablePeriodicSync(const bool periodicSyncOn)
{
    bool actionRequired = (periodicSyncOn != m_syncEnabled);

    if (!actionRequired)
        return;

    if (!isValid())
        return;

    if (periodicSyncOn) {
        m_synchronizer->instance().addHueObject(shared_from_this());
        m_syncEnabled = true;
    }
    else {
        m_synchronizer->instance().removeHueObject(shared_from_this());
        m_syncEnabled = false;
    }
}

QMap<int, QJsonObject> HueAbstractObject::parseJson(QJsonObject json)
{
    QMap<int, QJsonObject> map;

    int id = 1;
    QJsonValue jsonVal;

    while ((jsonVal = json.value(QString::number(id))) != QJsonValue::Undefined) {
        map.insert(id, jsonVal.toObject());
        id++;
    }

    return map;
}

bool HueAbstractObject::sendRequest(HueRequest request)
{
    HueReply reply;
    return sendRequest(request, reply);
}

bool HueAbstractObject::sendRequest(HueRequest request, HueReply& reply)
{
    reply = m_bridge->sendRequest(request);

    if (reply.isValid() && !reply.timedOut()) {
        return true;
    }
    else {
        qDebug() << "INVALID HUE REPLY";
        qDebug() << "=============================";
        qDebug() << "Is valid: " << reply.isValid();
        qDebug() << "Timed out: " << reply.timedOut();
        qDebug() << "HTTP status: " << reply.getHttpStatus();
        qDebug() << "Error type: " << reply.getErrorType();
        qDebug() << "Error address: " << reply.getErrorAddress();
        qDebug() << "Error description: " << reply.getErrorDescription();
        qDebug() << "=============================";
    }

    return false;
}

void HueAbstractObject::setBridge(HueBridge *bridge)
{
    m_bridge = bridge;
}

HueBridge* HueAbstractObject::getBridge() const
{
    return m_bridge;
}
