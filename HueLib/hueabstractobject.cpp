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

bool HueAbstractObject::turnOn(bool on)
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

bool HueAbstractObject::turnOff(bool off)
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

bool HueAbstractObject::setHue(int hue)
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

bool HueAbstractObject::setSaturation(int saturation)
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

bool HueAbstractObject::setBrightness(int brightness)
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

bool HueAbstractObject::setColorTemp(int colorTemp)
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

bool HueAbstractObject::setXY(double x, double y)
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

bool HueAbstractObject::setAlert(HueAlert alert)
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

bool HueAbstractObject::setEffect(HueEffect effect)
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

void HueAbstractObject::enablePeriodicSync(const HueObjectList<HueAbstractObject>& list, bool periodicSyncOn)
{
    // must be down-cast
    /*
    auto sharedToThis = list.fetch(this->ID());

    if (periodicSyncOn && !m_syncEnabled) {
        m_synchronizer->instance().addHueObject(sharedToThis);
        m_syncEnabled = true;
    }
    else if (!periodicSyncOn && m_syncEnabled){
        if (m_syncEnabled)
            m_synchronizer->instance().removeHueObject(sharedToThis);

        m_syncEnabled = false;
    }
    */
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
