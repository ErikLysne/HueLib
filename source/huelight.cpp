#include "huelight.h"

#include "huebridge.h"
#include "huerequest.h"
#include "huereply.h"

/*!
 * \class HueLight
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief The HueLight class is used to interface with a single Hue light.
 *
 * A HueLight object mirrors the state of a single Hue light connected to the network.
 * HueLight objects cannot be constructed directly because the constructor of HueLight
 * is private. Instead, a list of HueLight objects (a \l HueLightList) is returned
 * by the static function \l discoverLights(). A \l HueBridge object must be constructed
 * before any HueLight objects can be constructed.
 *
 * The following minimum working example turns off all lights on the network, then turns
 * on the light with \e ID 1 and enables the color loop effect.
 *
 * \code
 *  // Create HueBridge
 *  HueBridge* bridge = new HueBridge("10.0.1.14");
 *
 *  // Link to bridge
 *  bridge->link();
 *
 *  // Discover lights on the network
 *  HueLightList* lights = new HueLightList(HueLight::discoverLights(bridge));
 *
 *  // Turn off all lights
 *  for (auto light : *lights)
 *      light->turnOff();
 *
 *  // Turn on light 1 and set effect to color loop
 *  lights->fetch(1)->turnOn();
 *  lights->fetch(1)->setEffect(HueLight::HueEffect::ColorLoop);
 * \endcode
 *
 * As can be seen from the example above, lights can be changed by directly calling
 * \e set functions on the HueLight objects. Below is a list of \e set functions that
 * work with HueLight objects.
 *
 * \table
 * \header
 *  \li Function
 *  \li Description
 *  \li Range
 * \row
 *  \li \l turnOn(const bool on = true)
 *  \li Turns the light on.
 *  \li \c true or \c false
 *
 * \row
 *  \li \l turnOff(const bool off = true)
 *  \li Turns the light off.
 *  \li \c true or \c false
 *
 * \row
 *  \li \l setHue(const int hue)
 *  \li Sets the hue of the light.
 *  \li 0 - 65535
 *
 * \row
 *  \li \l setSaturation(const int saturation)
 *  \li Sets the saturation of the light.
 *  \li 0 - 254
 *
 * \row
 *  \li \l setBrightness(const int brightness)
 *  \li Sets the brightness of the light.
 *  \li 1 - 254
 *
 * \row
 *  \li \l setColorTemp(const int colorTemp)
 *  \li Sets the color temperature of the light.
 *  \li 150 - 500
 *      (corresponds to 6500K - 2000K)
 *
 * \row
 *  \li \l setXY(const double x, const double y)
 *  \li Sets the X and Y color coordinates in CIE color space.
 *  \li 0 - 1
 *
 * \row
 *  \li {1, 3} \l setAlert(const HueAlert alert)
 *  \li {1, 3} Sets an alert on the light.
 *  \li \l NoAlert
 *  \row
 *  \li \l BreatheSingle
 *  \row
 *  \li \l Breathe15Sec
 *
 * \row
 *  \li {1, 2} \l setEffect(const HueEffect effect);
 *  \li {1, 2} Sets an effect on the light.
 *  \li \l NoEffect
 *  \row
 *  \li \l ColorLoop
 *
 * \endtable
 *
 * Data can be retreved from HueLight by calling the functions in the table below.
 * \note The data follows the convension of the Hue API, where some data is nested
 * and some is not. For consistency, all data functions return an object of a data
 * class, where \e get functions can be called to retreive the data - even when
 * only one \e get function exists. The exception is \l ID() which returns the ID of
 * the light as an \c int.
 *
 * \table
 * \header
 *  \li Function
 *  \li Description
 *  \li \e get functions
 * \row
 *  \li {1, 13} \l Light::State state()
 *  \li {1, 13} Describes the state of the light.
 *  \li \c bool \l Light::State::isOn()
 *  \row
 *  \li \c bool \l Light::State::isReachable()
 *  \row
 *  \li \c int \l Light::State::getBrightness()
 *  \row
 *  \li \c int \l Light::State::getHue()
 *  \row
 *  \li \c int \l Light::State::getSaturation()
 *  \row
 *  \li \c int \l Light::State::getColorTemp()
 *  \row
 *  \li \c double \l Light::State::getXValue()
 *  \row
 *  \li \c double \l Light::State::getYValue()
 *  \row
 *  \li \c QString \l Light::State::getEffect()
 *  \row
 *  \li \c QString \l Light::State::getAlert()
 *  \row
 *  \li \c QString \l Light::State::getColorMode()
 *  \row
 *  \li \c QString \l Light::State::getMode()
 *  \row
 *  \li
 *
 * \row
 *  \li {1, 1} \l Light::Name name()
 *  \li {1, 1} Name of the light.
 *  \li \c QString \l Light::Name::getName()
 *
 * \row
 *  \li {1, 1} \l Light::Type type()
 *  \li {1, 1} Describes the type of light.
 *  \li \c QString \l Light::Type::getType()
 *
 * \row
 *  \li {1, 1} \l Light::UniqueID uniqueID()
 *  \li {1, 1} Provides an unique ID for the light.
 *  \li \c QString \l Light::UniqueID::getUniqueID()
 *
 * \row
 *  \li {1, 1} \l Light::SoftwareVersion softwareVersion()
 *  \li {1, 1} Software version on the light.
 *  \li \c QString \l Light::SoftwareVersion::getSoftwareVersion()
 *
 * \row
 *  \li {1, 3} \l Light::SoftwareUpdate softwareUpdate()
 *  \li {1, 3} Software version on the light.
 *  \li \c QString \l Light::SoftwareUpdate::getState()
 *  \row
 *  \li \c QString \l Light::SoftwareUpdate::getLastInstall()
 *  \row
 *  \li
 *
 * \row
 *  \li {1, 1} \l Light::SoftwareConfigID softwareConfigID()
 *  \li {1, 1} Provides a software config ID for the light.
 *  \li \c QString \l Light::SoftwareConfigID::getSoftwareConfigID()
 *
 * \row
 *  \li {1, 1} \l Light::ProductName productName()
 *  \li {1, 1} Name of the light product.
 *  \li \c QString \l Light::ProductName::getProductName()
 *
 * \row
 *  \li {1, 1} \l Light::Manufacturer manufacturer()
 *  \li {1, 1} Manufacturer of the light.
 *  \li \c QString \l Light::Manufacturer::getManufacturer()
 *
 * \row
 *  \li {1, 1} \l Light::ProductID productID()
 *  \li {1, 1} Provides a product ID for the light.
 *  \li \c QString \l Light::ProductID::getProductID()
 *
 * \row
 *  \li {1, 7} \l Light::Config config()
 *  \li {1, 7} Configuration of the light.
 *  \li \c QString \l Light::Config::getArchetype()
 *  \row
 *  \li \c QString \l Light::Config::getFunction()
 *  \row
 *  \li \c QString \l Light::Config::getDirection()
 *  \row
 *  \li \l Light::Config::Startup \l Light::Config::getStartup()
 *  \row
 *  \li \c QString \l Light::Config::Startup::getMode()
 *  \row
 *  \li \c bool \l Light::Config::Startup::getConfigured()
 * \endtable
 *
 */

/*!
 * \fn HueLight::HueLight()
 *
 * Creates an empty HueLight.
 *
 * \note should not be called explicitly.
 *
 * \sa discoverLights()
 *
 */
HueLight::HueLight()
    : HueAbstractObject(nullptr)
    , m_ID()
    , m_state()
    , m_name()
    , m_type()
    , m_uniqueID()
    , m_softwareVersion()
    , m_softwareUpdate()
    , m_softwareConfigID()
    , m_productName()
    , m_manufacturer()
    , m_productID()
    , m_config()
    , m_validConstructor(false)
{

}

/*!
 * \fn HueLight(HueBridge* bridge)
 *
 * Creates an empty HueLight with a \l HueBridge reference
 * provided by \a bridge.
 *
 * \note should not be called explicitly.
 *
 * \sa discoverLights()
 *
 */
HueLight::HueLight(HueBridge* bridge)
    : HueAbstractObject(bridge)
    , m_ID()
    , m_state()
    , m_name()
    , m_type()
    , m_uniqueID()
    , m_softwareVersion()
    , m_softwareUpdate()
    , m_softwareConfigID()
    , m_productName()
    , m_manufacturer()
    , m_productID()
    , m_config()
    , m_validConstructor(false)
{

}

HueLight::HueLight(HueBridge* bridge,
                   int ID,
                   Light::State state, Light::Name name,
                   Light::Type type, Light::UniqueID uniqueID,
                   Light::SoftwareVersion softwareVersion,
                   Light::SoftwareUpdate softwareUpdate,
                   Light::SoftwareConfigID softwareConfigID,
                   Light::ProductName productName,
                   Light::Manufacturer manufacturer,
                   Light::ProductID productID,
                   Light::Config config)
    : HueAbstractObject(bridge)
    , m_ID(ID)
    , m_state(state)
    , m_name(name)
    , m_type(type)
    , m_uniqueID(uniqueID)
    , m_softwareVersion(softwareVersion)
    , m_softwareUpdate(softwareUpdate)
    , m_softwareConfigID(softwareConfigID)
    , m_productName(productName)
    , m_manufacturer(manufacturer)
    , m_productID(productID)
    , m_config(config)
    , m_validConstructor(true)
{

}

HueLight::HueLight(const HueLight& rhs)
    : HueAbstractObject(rhs.getBridge())
    , m_ID(rhs.m_ID)
    , m_state(rhs.m_state)
    , m_name(rhs.m_name)
    , m_type(rhs.m_type)
    , m_uniqueID(rhs.m_uniqueID)
    , m_softwareVersion(rhs.m_softwareVersion)
    , m_softwareUpdate(rhs.m_softwareUpdate)
    , m_softwareConfigID(rhs.m_softwareConfigID)
    , m_productName(rhs.m_productName)
    , m_manufacturer(rhs.m_manufacturer)
    , m_productID(rhs.m_productID)
    , m_config(rhs.m_config)
    , m_validConstructor(rhs.m_validConstructor)
{

}

HueLight HueLight::operator=(const HueLight &rhs)
{
    if (&rhs == this)
        return *this;

    this->setBridge(rhs.getBridge());
    this->setParent(rhs.parent());
    m_ID = rhs.m_ID;
    m_state = rhs.m_state;
    m_name = rhs.m_name;
    m_type = rhs.m_type;
    m_uniqueID = rhs.m_uniqueID;
    m_softwareVersion = rhs.m_softwareVersion;
    m_softwareUpdate = rhs.m_softwareUpdate;
    m_softwareConfigID = rhs.m_softwareConfigID;
    m_productName = rhs.m_productName;
    m_manufacturer = rhs.m_manufacturer;
    m_productID = rhs.m_productID;
    m_config = rhs.m_config;
    m_validConstructor = rhs.m_validConstructor;

    return *this;
}

/*!
 * \fn HueLightList HueLight::discoverLights(HueBridge* bridge)
 *
 * Returns a \l HueLightList of lights connected to the network.
 * This function is static and is used to generate HueLight objects
 * using the connection to \l HueBridge specified by \a bridge.
 *
 */
HueLightList HueLight::discoverLights(HueBridge* bridge)
{
    if (bridge == nullptr)
        return HueLightList();

    std::shared_ptr<LightVector> lights = std::make_shared<LightVector>();

    HueRequest request("lights", QJsonObject(), HueRequest::Get);
    HueReply reply = bridge->sendRequest(request, nullptr);

    if (!reply.isValid() || reply.timedOut() || reply.containsError()) {
        qDebug().noquote() << reply;
        return HueLightList();
    }

    QJsonObject json = reply.getJson();
    auto parsedJsonWithIDs = HueAbstractObject::parseJson(json);

    if (parsedJsonWithIDs.isEmpty())
        return HueLightList();

    for (auto iter = parsedJsonWithIDs.begin(); iter != parsedJsonWithIDs.end(); ++iter) {
        int ID = iter.key();
        QJsonObject json = iter.value();

        std::shared_ptr<HueLight> light = std::make_shared<HueLight>(bridge);

        if (HueLight::constructHueLight(ID, json, light))
            lights.get()->push_back(std::move(light));
    }

    return HueLightList(std::move(lights));
}

/*!
 * \fn Light::State HueLight::state() const
 *
 * Returns a \l Light::State object.
 *
 */
Light::State HueLight::state() const
{
    return m_state;
}

/*!
 * \fn Light::Name HueLight::name() const
 *
 * Returns a \l Light::Name object.
 *
 */
Light::Name HueLight::name() const
{
    return m_name;
}

/*!
 * \fn Light::Type HueLight::type() const
 *
 * Returns a \l Light::Type object.
 *
 */
Light::Type HueLight::type() const
{
    return m_type;
}

/*!
 * \fn Light::UniqueID HueLight::uniqueID() const
 *
 * Returns a \l Light::UniqueID object.
 *
 */
Light::UniqueID HueLight::uniqueID() const
{
    return m_uniqueID;
}

/*!
 * \fn Light::SoftwareVersion HueLight::softwareVersion() const
 *
 * Returns a \l Light::SoftwareVersion object.
 *
 */
Light::SoftwareVersion HueLight::softwareVersion() const
{
    return m_softwareVersion;
}

/*!
 * \fn Light::SoftwareUpdate HueLight::softwareUpdate() const
 *
 * Returns a \l Light::SoftwareUpdate object.
 *
 */
Light::SoftwareUpdate HueLight::softwareUpdate() const
{
    return m_softwareUpdate;
}

/*!
 * \fn Light::SoftwareConfigID HueLight::softwareConfigID() const
 *
 * Returns a \l Light::SoftwareConfigID object.
 *
 */
Light::SoftwareConfigID HueLight::softwareConfigID() const
{
    return m_softwareConfigID;
}

/*!
 * \fn Light::ProductName HueLight::productName() const
 *
 * Returns a \l Light::ProductName object.
 *
 */
Light::ProductName HueLight::productName() const
{
    return m_productName;
}

/*!
 * \fn Light::Manufacturer HueLight::manufacturer() const
 *
 * Returns a \l Light::Manufacturer object.
 *
 */
Light::Manufacturer HueLight::manufacturer() const
{
    return m_manufacturer;
}

/*!
 * \fn Light::ProductID HueLight::productID() const
 *
 * Returns a \l Light::ProductID object.
 *
 */
Light::ProductID HueLight::productID() const
{
    return m_productID;
}

/*!
 * \fn Light::Config HueLight::config() const
 *
 * Returns a \l Light::Config object.
 *
 */
Light::Config HueLight::config() const
{
    return m_config;
}

/*!
 * \fn bool HueLight::hasValidConstructor() const
 *
 * Returns \c true if HueLight was constructed properly.
 *
 * \sa isValid()
 *
 */
bool HueLight::hasValidConstructor() const
{
    return m_validConstructor;
}

/*!
 * \fn bool HueLight::isValid() const
 *
 * Equivalent to \l hasValidConstructor()
 *
 */
bool HueLight::isValid() const
{
    return hasValidConstructor();
}

/*!
 * \fn int HueLight::ID() const
 *
 * Returns an \c int with the ID of the light.
 *
 */
int HueLight::ID() const
{
    return m_ID;
}

/*!
 * \fn bool HueLight::synchronize()
 *
 * Updates HueLight to the lastest state available on \l HueBridge.
 *
 * \note only queries an individual HueLight from the bridge.
 * If all lights needs to be updated, it can be faster to delete the
 * \l HueLightList and call \l discoverLights() again.
 *
 * \sa discoverLights()
 *
 */
bool HueLight::synchronize()
{
    HueRequest syncRequest = makeGetRequest();
    HueReply syncReply;

    bool replyValid = sendRequest(syncRequest, syncReply);

    if (replyValid) {
        QJsonObject json = syncReply.getJson();
        std::shared_ptr<HueLight> synchronizedLight = std::make_shared<HueLight>(getBridge());

        if (constructHueLight(m_ID, json, synchronizedLight)) {
            if (synchronizedLight->hasValidConstructor()) {
                *this = *synchronizedLight.get();

                emit synchronized();
                return true;
            }
        }
    }

    return false;
}

bool HueLight::constructHueLight(int ID, QJsonObject json, std::shared_ptr<HueLight>& light)
{
    bool jsonIsValid =
            json.contains("state")              &
            json.contains("swupdate")           &
            json.contains("type")               &
            json.contains("name")               &
            json.contains("modelid")            &
            json.contains("manufacturername")   &
            json.contains("productname")        &
            json.contains("capabilities")       &
            json.contains("config")             &
            json.contains("uniqueid")           &
            json.contains("swversion")          &
            json.contains("swconfigid")         &
            json.contains("productid")          ;

    if(!jsonIsValid)
        return false;

    Light::State state(json["state"]);
    Light::Name name(json["name"]);
    Light::Type type(json["type"]);
    Light::UniqueID uniqueID(json["uniqueid"]);
    Light::SoftwareVersion softwareVersion(json["swversion"]);
    Light::SoftwareUpdate softwareUpdate(json["swupdate"]);
    Light::SoftwareConfigID softwareConfigID(json["swconfigid"]);
    Light::ProductName productName(json["productname"]);
    Light::Manufacturer manufacturer(json["manufacturername"]);
    Light::ProductID productID(json["productid"]);
    Light::Config config(json["config"]);

    HueLight* newLight = new HueLight(light->getBridge(),
                                      ID,
                                      state,
                                      name,
                                      type,
                                      uniqueID,
                                      softwareVersion,
                                      softwareUpdate,
                                      softwareConfigID,
                                      productName,
                                      manufacturer,
                                      productID,
                                      config);

    light.reset(newLight);

    return true;
}

HueRequest HueLight::makePutRequest(QJsonObject json)
{
    QString urlPath = "lights/" + QString::number(m_ID) + "/state";
    HueRequest::Method method = HueRequest::Put;

    return HueRequest(urlPath, json, method);
}

HueRequest HueLight::makeGetRequest()
{
    QString urlPath = "lights/" + QString::number(m_ID);
    HueRequest::Method method = HueRequest::Get;

    return HueRequest(urlPath, QJsonObject(), method);
}

void HueLight::updateOn(const bool on)
{
    m_state.setOn(on);
    emit valueUpdated();
}

void HueLight::updateHue(const int hue)
{
    m_state.setHue(hue);
    emit valueUpdated();
}

void HueLight::updateSaturation(const int saturation)
{
    m_state.setSaturation(saturation);
    emit valueUpdated();
}

void HueLight::updateBrightness(const int brightness)
{
    m_state.setBrightness(brightness);
    emit valueUpdated();
}

void HueLight::updateColorTemp(const int colorTemp)
{
    m_state.setColorTemp(colorTemp);
    emit valueUpdated();
}

void HueLight::updateXY(const double x, const double y)
{
    m_state.setXValue(x);
    m_state.setYValue(y);
    emit valueUpdated();
}

void HueLight::updateAlert(const HueAlert alert)
{
    QString alertString;
    switch (alert) {
    case HueAlert::NoAlert:
        alertString = "none";
        break;
    case HueAlert::BreatheSingle:
        alertString = "select";
        break;
    case HueAlert::Breathe15Sec:
        alertString = "lselect";
        break;
    }
    m_state.setAlert(alertString);
    emit valueUpdated();
}

void HueLight::updateEffect(const HueEffect effect)
{
    QString effectString;
    switch (effect) {
    case HueEffect::NoEffect:
        effectString = "none";
        break;
    case HueEffect::ColorLoop:
        effectString = "colorloop";
        break;
    }
    m_state.setEffect(effectString);
    emit valueUpdated();
}
