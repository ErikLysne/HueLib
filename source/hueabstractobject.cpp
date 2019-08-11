#include "hueabstractobject.h"

#include <QJsonArray>

#include "huebridge.h"
#include "huerequest.h"
#include "huereply.h"
#include "huesynchronizer.h"


/*!
 * \class HueAbstractObject
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief HueAbstractObject is the abstract base class for \l HueLight and \l HueGroup.
 *
 * HueAbstractObject specifies the common interface for \l HueLight and \l HueGroup.
 * The class implements a series of public \e set functions that changes properties
 * of the derived class objects by sending requests to \l HueBridge. The \e set
 * functions call corresponding \e update functions which are pure virtual and
 * must be overloaded by the derived classes.
 */

/*!
 * \enum HueAbstractObject::HueAlert
 * This enum defines the legal values of \l setAlert().
 *
 * \value NoAlert
 *      No alert (normal operation).
 *
 * \value BreatheSingle
 *      Performs a single breathe cycle.
 *
 * \value Breathe15Sec
 *      Performs breathe cycles for 15 sec.
 */

/*!
 * \enum HueAbstractObject::HueEffect
 * This enum defines the legal values of \l setEffect().
 *
 * \value NoEffect
 *      No alert (normal operation).
 *
 * \value ColorLoop
 *      Hue continuously changes value.
 */

/*!
 * \fn HueAbstractObject::HueAbstractObject(HueBridge* bridge)
 *
 * Base constructor for derived clases. Sets parent to \c nullptr
 * and bridge specified by \a bridge.
 *
 * \note should not be called explicitly.
 *
 */
HueAbstractObject::HueAbstractObject(HueBridge* bridge)
    : QObject(nullptr)
    , m_bridge(bridge)
{

}

/*!
 * \fn bool HueAbstractObject::turnOn(const bool on)
 *
 * Turns the light on or off when \a on is \c true or \c false respectively.
 *
 * Returns true if update was successful.
 *
 * \sa turnOff()
 */
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

/*!
 * \fn bool HueAbstractObject::turnOff(const bool off)
 *
 * Turns the light off or on when \a off is \c true or \c false respectively.
 *
 * Returns true if update was successful.
 *
 * \sa turnOn()
 */
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

/*!
 * \fn bool HueAbstractObject::setHue(const int hue)
 *
 * Sets the hue as specified by \a hue.
 *
 * Range for \a hue is 0 - 65535.
 *
 * Returns true if update was successful.
 *
 * \sa setSaturation()
 */
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

/*!
 * \fn bool HueAbstractObject::setSaturation(const int saturation)
 *
 * Sets the saturation as specified by \a saturation.
 *
 * Range for \a saturation is 0 - 254.
 *
 * Returns true if update was successful.
 *
 * \sa setHue()
 */
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

/*!
 * \fn bool HueAbstractObject::setBrightness(const int brightness)
 *
 * Sets the brightness as specified by \a brightness.
 *
 * Range for \a brightness is 1 - 254.
 *
 * Returns true if update was successful.
 *
 */
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

/*!
 * \fn bool HueAbstractObject::setColorTemp(const int colorTemp)
 *
 * Sets the color temperature as specified by \a colorTemp.
 *
 * Range for \a colorTemp is 150 - 500 which corresponds
 * to 6500K - 2000K.
 *
 * Returns true if update was successful.
 *
 */
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

/*!
 * \fn bool HueAbstractObject::setXY(const double x, const double y)
 *
 * Sets the X and Y color coordinates in CIE color space as
 * specified by \a x and \a y.
 *
 * Range for \a x and \a y is 0.0 - 1.0.
 *
 * Returns true if update was successful.
 *
 */
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

/*!
 * \fn bool HueAbstractObject::setAlert(const HueAlert alert)
 *
 * Sets alert as specified by \a alert.
 *
 * Returns true if update was successful.
 *
 * \sa HueAlert
 *
 */
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

/*!
 * \fn bool HueAbstractObject::setEffect(const HueEffect effect)
 *
 * Sets effect as specified by \a effect.
 *
 * Returns true if update was successful.
 *
 * \sa HueEffect
 *
 */
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

/*!
 * \fn void HueAbstractObject::enablePeriodicSync(const bool periodicSyncOn)
 *
 * Enables or disables periodic synchronization with bridge as specified by
 * \a periodicSyncOn. The synchronization interval can be set by the static
 * function \l HueSynchronizer::setSyncInterval().
 *
 * \sa HueSynchronizer
 *
 */
void HueAbstractObject::enablePeriodicSync(const bool periodicSyncOn)
{
    if (!isValid())
        return;

    if (periodicSyncOn) {
        m_synchronizer->instance().addHueObject(shared_from_this());
    }
    else {
        m_synchronizer->instance().removeHueObject(shared_from_this());
    }
}

/*!
 * \fn QMap<int, QJsonObject> HueAbstractObject::parseJson(QJsonObject json)
 *
 * Parses the JSON specified by \a json received when calling \e discover functions
 * implemented on the derived classes.
 *
 * Returns a \e QMap where the key is an \c int with the ID and the value
 * is a \e QJsonObject containing the JSON for the object with the corresponding
 * ID.
 *
 * \note should not be called explicitly.
 *
 * \sa HueLight::discoverLights(), HueGroup::discoverGroups()
 *
 */
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

/*!
 * \fn bool HueAbstractObject::sendRequest(HueRequest request)
 *
 * Sends the request specified by \a request to the bridge.
 *
 * Returns \c true if the reply is valid, does not contain an error
 * in the returned JSON, and did not timeout.
 *
 * \note should not be called explicitly.
 *
 * \sa HueRequest, sendRequest(HueRequest request, HueReply& reply)
 *
 */
bool HueAbstractObject::sendRequest(HueRequest request)
{
    HueReply reply;
    return sendRequest(request, reply);
}

/*!
 * \fn bool HueAbstractObject::sendRequest(HueRequest request, HueReply& reply)
 *
 * Sends the request specified by \a request to the bridge.
 *
 * Returns \c true if the reply is valid, does not contain an error
 * in the returned JSON, and did not timeout. \a reply is updated
 * with the reply.
 *
 * \note should not be called explicitly.
 *
 * \sa HueRequest, HueReply, sendRequest()
 *
 */
bool HueAbstractObject::sendRequest(HueRequest request, HueReply& reply)
{
    reply = m_bridge->sendRequest(request, this);

    if (reply.isValid() && !reply.timedOut() && !reply.containsError()) {
        return true;
    }

    qDebug().noquote() << reply;
    return false;
}

/*!
 * \fn void HueAbstractObject::setBridge(HueBridge *bridge)
 *
 * Sets the \l HueBridge as specified by \a bridge.
 *
 * \note should not be called explicitly.
 *
 */
void HueAbstractObject::setBridge(HueBridge* bridge)
{
    m_bridge = bridge;
}

/*!
 * \fn HueBridge* HueAbstractObject::getBridge() const
 *
 * \note should not be called explicitly.
 *
 * Returns a reference to the \l HueBridge associated with the object.
 *
 */
HueBridge* HueAbstractObject::getBridge() const
{
    return m_bridge;
}

/*!
 * \fn virtual ~HueAbstractObject()
 *
 * Virtual destructor.
 *
 */

/*!
 * \fn virtual HueRequest HueAbstractObject::makePutRequest(QJsonObject json)
 *
 * Pure virtual function. Must be overloaded.
 *
 * Should return a \l HueRequest with \l HueRequest::Method set to \l HueRequest::Put
 * and JSON specified by \a json.
 *
 */

/*!
 * \fn virtual HueRequest HueAbstractObject::makeGetRequest()
 *
 * Pure virtual function. Must be overloaded.
 *
 * Should return a \l HueRequest with \l HueRequest::Method set to \l HueRequest::Get.
 *
 */

/*!
 * \fn virtual bool HueAbstractObject::hasValidConstructor() const
 *
 * Pure virtual function. Must be overloaded.
 *
 * Should return true if object has valid constructor.
 *
 */

/*!
 * \fn virtual bool HueAbstractObject::isValid() const
 *
 * Pure virtual function. Must be overloaded.
 *
 * Equivalent to \l hasValidConstructor().
 *
 * Should return true if object has valid constructor.
 *
 */

/*!
 * \fn virtual int HueAbstractObject::ID() const
 *
 * Pure virtual function. Must be overloaded.
 *
 * Should return the ID of the object.
 *
 */

/*!
 * \fn virtual bool HueAbstractObject::synchronize()
 *
 * Pure virtual function. Must be overloaded.
 *
 * Synchronizes the object.
 *
 * Should return true if synchronization was successful.
 *
 */

/*!
 * \fn virtual void HueAbstractObject::updateOn(const bool on)
 *
 * Pure virtual function. Must be overloaded.
 *
 * Should set \e on property of object as specified by \a on.
 *
 */

/*!
 * \fn virtual void HueAbstractObject::updateHue(const int hue)
 *
 * Pure virtual function. Must be overloaded.
 *
 * Should set \e hue property of object as specified by \a hue.
 *
 */

/*!
 * \fn virtual void HueAbstractObject::updateSaturation(const int saturation)
 *
 * Pure virtual function. Must be overloaded.
 *
 * Should set \e saturation property of object as specified by \a saturation.
 *
 */

/*!
 * \fn virtual void HueAbstractObject::updateBrightness(const int brightness)
 *
 * Pure virtual function. Must be overloaded.
 *
 * Should set \e brightness property of object as specified by \a brightness.
 *
 */

/*!
 * \fn virtual void HueAbstractObject::updateColorTemp(const int colorTemp)
 *
 * Pure virtual function. Must be overloaded.
 *
 * Should set \e colorTemp property of object as specified by \a colorTemp.
 *
 */

/*!
 * \fn virtual void HueAbstractObject::updateXY(const double x, const double y)
 *
 * Pure virtual function. Must be overloaded.
 *
 * Should set \e x and \e y properties of object as specified by \a x and \a y.
 *
 */

/*!
 * \fn virtual void HueAbstractObject::updateAlert(const HueAlert alert)
 *
 * Pure virtual function. Must be overloaded.
 *
 * Should set \e alert property of object as specified by \a alert.
 *
 */

/*!
 * \fn virtual void HueAbstractObject::updateEffect(const HueEffect effect)
 *
 * Pure virtual function. Must be overloaded.
 *
 * Should set \e effect property of object as specified by \a effect.
 *
 */

/*!
 * \fn void HueAbstractObject::synchronized()
 *
 * Signal should be emitted from object of derived class after object has been
 * successfully synchronized.
 *
 * \sa HueLight::synchronize(), HueGroup::synchronize()
 *
 */

/*!
 * \fn void HueAbstractObject::valueUpdated()
 *
 * Signal should be emitted from object of derived class after object has been
 * updated.
 *
 */
