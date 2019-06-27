#include "hueabstractobject.h"
#include <QJsonArray>

QTimer* HueAbstractObject::m_syncTimer = new QTimer();

HueAbstractObject::HueAbstractObject(HueBridge* bridge) :
    QObject(bridge),
    m_bridge(bridge),
    m_syncIntervalSeconds(5)
{

}

bool HueAbstractObject::turnOn()
{
    QJsonObject json {
        {"on", true}
    };

    HueRequest request = makePutRequest(json);
    return sendRequest(request);
}

bool HueAbstractObject::turnOff()
{
    QJsonObject json {
        {"on", false}
    };

    HueRequest request = makePutRequest(json);
    return sendRequest(request);
}

bool HueAbstractObject::setHue(int hue)
{
    QJsonObject json {
        {"hue", hue}
    };

    HueRequest request = makePutRequest(json);
    return sendRequest(request);
}

bool HueAbstractObject::setSaturation(int saturation)
{
    QJsonObject json {
        {"sat", saturation}
    };

    HueRequest request = makePutRequest(json);
    return sendRequest(request);
}

bool HueAbstractObject::setBrightness(int brightness)
{
    QJsonObject json {
        {"bri", brightness}
    };

    HueRequest request = makePutRequest(json);
    return sendRequest(request);
}

bool HueAbstractObject::setColorTemp(int colorTemp)
{
    QJsonObject json {
        {"ct", colorTemp}
    };

    HueRequest request = makePutRequest(json);
    return sendRequest(request);
}

bool HueAbstractObject::setXY(double x, double y)
{
    QJsonArray xy = {x, y};
    QJsonObject json {
        {"ct", xy}
    };

    HueRequest request = makePutRequest(json);
    return sendRequest(request);
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
    return sendRequest(request);
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
    return sendRequest(request);
}

void HueAbstractObject::synchronizePeriodically(bool enable)
{
    if (enable) {
        connect(m_syncTimer, &QTimer::timeout,
                this, &HueAbstractObject::timeToSynchronize);
        m_syncTimer->setSingleShot(false);
        m_syncTimer->start(m_syncIntervalSeconds*1000);
    }
    else {
        m_syncTimer->stop();
    }
}


void HueAbstractObject::setSynchronizationInterval(int intervalSeconds)
{
    m_syncIntervalSeconds = intervalSeconds;
    synchronizePeriodically();
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

void HueAbstractObject::timeToSynchronize()
{
    synchronize();
}
