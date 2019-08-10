#include "huelight.h"

#include "huebridge.h"
#include "huerequest.h"
#include "huereply.h"

/*!
 * \class HueLight
 * \ingroup HueLib
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
 *  \li \l setAlert(const HueAlert alert)
 *  \li Sets an alert on the light.
 *  \li \l HueAlert::NoAlert,
 *      \l HueAlert::BreatheSingle or
 *      \l HueAlert::Breathe15sec
 *
 * \row
 *  \li \l setEffect(const HueEffect effect);
 *  \li Sets an effect on the light.
 *  \li \l HueEffect::NoEffect or
 *      \l HueEffect::ColorLoop
 *
 * \endtable
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

Light::State HueLight::state() const
{
    return m_state;
}

Light::Name HueLight::name() const
{
    return m_name;
}

Light::Type HueLight::type() const
{
    return m_type;
}

Light::UniqueID HueLight::uniqueID() const
{
    return m_uniqueID;
}

Light::SoftwareVersion HueLight::softwareVersion() const
{
    return m_softwareVersion;
}

Light::SoftwareUpdate HueLight::softwareUpdate() const
{
    return m_softwareUpdate;
}

Light::SoftwareConfigID HueLight::softwareConfigID() const
{
    return m_softwareConfigID;
}

Light::ProductName HueLight::productName() const
{
    return m_productName;
}

Light::Manufacturer HueLight::manufacturer() const
{
    return m_manufacturer;
}

Light::ProductID HueLight::productID() const
{
    return m_productID;
}

Light::Config HueLight::config() const
{
    return m_config;
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

bool HueLight::hasValidConstructor() const
{
    return m_validConstructor;
}

bool HueLight::isValid() const
{
    return hasValidConstructor();
}

int HueLight::ID() const
{
    return m_ID;
}

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
}

void HueLight::updateHue(const int hue)
{
    m_state.setHue(hue);
}

void HueLight::updateSaturation(const int saturation)
{
    m_state.setSaturation(saturation);
}

void HueLight::updateBrightness(const int brightness)
{
    m_state.setBrightness(brightness);
}

void HueLight::updateColorTemp(const int colorTemp)
{
    m_state.setColorTemp(colorTemp);
}

void HueLight::updateXY(const double x, const double y)
{
    m_state.setXValue(x);
    m_state.setYValue(y);
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
}
