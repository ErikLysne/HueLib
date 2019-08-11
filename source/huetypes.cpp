#include "huetypes.h"

#include <QJsonArray>

/* =====================================
 * =============== LIGHT ===============
 * =====================================
 *
 */

/*!
 * \namespace Light
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief Contains data classes for \l HueLight.
 *
 */

// ---------- STATE ----------

/*!
 * \class Light::State
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief Describes the state of a \l HueLight.
 *
 */

/*!
 * \fn Light::State::State()
 *
 * Constructs an empty State object.
 *
 */
Light::State::State()
    : m_on(false)
    , m_reachable(false)
    , m_brightness(0)
    , m_hue(0)
    , m_saturation(0)
    , m_colorTemp(0)
    , m_xValue(0)
    , m_yValue(0)
    , m_effect("")
    , m_alert("")
    , m_colorMode("")
    , m_mode("")
{

}

/*!
 * \fn Light::State::State(const QJsonValue json)
 *
 * Constructs a State object by parsing JSON specified
 * by \a json.
 *
 */
Light::State::State(const QJsonValue json)
{
    QJsonObject stateJson = json.toObject();

    bool jsonIsValid =
            stateJson.contains("on")        &
            stateJson.contains("reachable") &
            stateJson.contains("bri")       &
            stateJson.contains("hue")       &
            stateJson.contains("sat")       &
            stateJson.contains("ct")        &
            stateJson.contains("xy")        &
            stateJson.contains("effect")    &
            stateJson.contains("alert")     &
            stateJson.contains("colormode") &
            stateJson.contains("mode")      ;

    if (jsonIsValid) {
        m_on            = stateJson["on"].toBool();
        m_reachable     = stateJson["reachable"].toBool();
        m_brightness    = stateJson["bri"].toInt();
        m_hue           = stateJson["hue"].toInt();
        m_saturation    = stateJson["sat"].toInt();
        m_colorTemp     = stateJson["ct"].toInt();
        m_xValue        = stateJson["xy"].toArray()[0].toDouble();
        m_yValue        = stateJson["xy"].toArray()[1].toDouble();
        m_effect        = stateJson["effect"].toString();
        m_alert         = stateJson["alert"].toString();
        m_colorMode     = stateJson["colormode"].toString();
        m_mode          = stateJson["mode"].toString();
    }
    else {
        Light::State();
    }
}

/*!
 * \fn bool Light::State::isOn() const
 *
 * Returns \c true if light is on.
 *
 */
bool Light::State::isOn() const
{
    return m_on;
}

/*!
 * \fn bool Light::State::isReachable() const
 *
 * Returns \c true if light is reachable.
 *
 */
bool Light::State::isReachable() const
{
    return m_reachable;
}

/*!
 * \fn int Light::State::getBrightness() const
 *
 * Returns brightness as an \c int.
 *
 */
int Light::State::getBrightness() const
{
    return m_brightness;
}

/*!
 * \fn int Light::State::getHue() const
 *
 * Returns hue as an \c int.
 *
 */
int Light::State::getHue() const
{
    return m_hue;
}

/*!
 * \fn int Light::State::getSaturation() const
 *
 * Returns saturation as an \c int.
 *
 */
int Light::State::getSaturation() const
{
    return m_saturation;
}

/*!
 * \fn int Light::State::getColorTemp() const
 *
 * Returns color temperature as an \c int.
 *
 */
int Light::State::getColorTemp() const
{
    return m_colorTemp;
}

/*!
 * \fn double Light::State::getXValue() const
 *
 * Returns X coordinate in CIE color space
 * as a \c double.
 *
 */
double Light::State::getXValue() const
{
    return m_xValue;
}

/*!
 * \fn double Light::State::getYValue() const
 *
 * Returns Y coordinate in CIE color space
 * as a \c double.
 *
 */
double Light::State::getYValue() const
{
    return m_yValue;
}

/*!
 * \fn QString Light::State::getEffect() const
 *
 * Returns effect as a \e QString.
 *
 */
QString Light::State::getEffect() const
{
    return m_effect;
}

/*!
 * \fn QString Light::State::getAlert() const
 *
 * Returns alert as a \e QString.
 *
 */
QString Light::State::getAlert() const
{
    return m_alert;
}

/*!
 * \fn QString Light::State::getColorMode() const
 *
 * Returns color mode as a \e QString.
 *
 */
QString Light::State::getColorMode() const
{
    return m_colorMode;
}

/*!
 * \fn QString Light::State::getMode() const
 *
 * Returns mode as a \e QString.
 *
 */
QString Light::State::getMode() const
{
    return m_mode;
}

/*!
 * \fn void Light::State::setOn(const bool on)
 *
 * Sets on as specified by \a on.
 *
 */
void Light::State::setOn(const bool on)
{
    m_on = on;
}

/*!
 * \fn void Light::State::setReachable(const bool reachable)
 *
 * Sets reachable as specified by \a reachable.
 *
 */
void Light::State::setReachable(const bool reachable)
{
    m_reachable = reachable;
}

/*!
 * \fn void Light::State::setBrightness(const int brightness)
 *
 * Sets brightness as specified by \a brightness.
 *
 */
void Light::State::setBrightness(const int brightness)
{
    m_brightness = brightness;
}

/*!
 * \fn void Light::State::setHue(const int hue)
 *
 * Sets hue as specified by \a hue.
 *
 */
void Light::State::setHue(const int hue)
{
    m_hue = hue;
}

/*!
 * \fn void Light::State::setSaturation(const int saturation)
 *
 * Sets saturation as specified by \a saturation.
 *
 */
void Light::State::setSaturation(const int saturation)
{
    m_saturation = saturation;
}

/*!
 * \fn void Light::State::setColorTemp(const int colorTemp)
 *
 * Sets color temperature as specified by \a colorTemp.
 *
 */
void Light::State::setColorTemp(const int colorTemp)
{
    m_colorTemp = colorTemp;
}

/*!
 * \fn void Light::State::setXValue(const double xValue)
 *
 * Sets X coordinate in CIE color space
 * as specified by \a xValue.
 *
 */
void Light::State::setXValue(const double xValue)
{
    m_xValue = xValue;
}

/*!
 * \fn void Light::State::setYValue(const double yValue)
 *
 * Sets Y coordinate in CIE color space
 * as specified by \a yValue.
 *
 */
void Light::State::setYValue(const double yValue)
{
    m_yValue = yValue;
}

/*!
 * \fn Light::State::setEffect(const QString effect)
 *
 * Sets effect as specified by \a effect.
 *
 */
void Light::State::setEffect(const QString effect)
{
    m_effect = effect;
}

/*!
 * \fn void Light::State::setAlert(const QString alert)
 *
 * Sets alert as specified by \a alert.
 *
 */
void Light::State::setAlert(const QString alert)
{
    m_alert = alert;
}

/*!
 * \fn void Light::State::setColorMode(const QString colorMode)
 *
 * Sets color mode as specified by \a colorMode.
 *
 */
void Light::State::setColorMode(const QString colorMode)
{
    m_colorMode = colorMode;
}

/*!
 * \fn void Light::State::setMode(const QString mode)
 *
 * Sets mode as specified by \a mode.
 *
 */
void Light::State::setMode(const QString mode)
{
    m_mode = mode;
}

// ---------- TYPE ----------

/*!
 * \class Light::Type
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief Describes the type of a \l HueLight.
 *
 */

/*!
 * \fn Light::Type::Type()
 *
 * Constructs an empty Type object.
 *
 */
Light::Type::Type()
    : m_type("")
{

}

/*!
 * \fn Light::Type::Type(const QJsonValue json)
 *
 * Constructs a Type object by parsing JSON specified
 * by \a json.
 *
 */
Light::Type::Type(const QJsonValue json)
{
    m_type = json.toString();
}

/*!
 * \fn QString Light::Type::getType() const
 *
 * Returns type as a \e QString.
 *
 */
QString Light::Type::getType() const
{
    return m_type;
}

/*!
 * \fn void Light::Type::setType(const QString type)
 *
 * Sets type as specified by \a type.
 *
 */
void Light::Type::setType(const QString type)
{
    m_type = type;
}

// ---------- Name ----------

/*!
 * \class Light::Name
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief Describes the name of a \l HueLight.
 *
 */

/*!
 * \fn Light::Name::Name()
 *
 * Constructs an empty Name object.
 *
 */
Light::Name::Name()
    : m_name("")
{

}

/*!
 * \fn Light::Name::Name(const QJsonValue json)
 *
 * Constructs a Name object by parsing JSON specified
 * by \a json.
 *
 */
Light::Name::Name(const QJsonValue json)
{
    m_name = json.toString();
}

/*!
 * \fn QString Light::Name::getName() const
 *
 * Returns name as a \e QString.
 *
 */
QString Light::Name::getName() const
{
    return m_name;
}

/*!
 * \fn void Light::Name::setName(const QString name)
 *
 * Sets name as specified by \a name.
 *
 */
void Light::Name::setName(const QString name)
{
    m_name = name;
}

// ---------- Unique ID ----------

/*!
 * \class Light::UniqueID
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief Describes the unique ID of a \l HueLight.
 *
 */

/*!
 * \fn Light::UniqueID::UniqueID()
 *
 * Constructs an empty UniqueID object.
 *
 */
Light::UniqueID::UniqueID()
    : m_uniqueID("")
{

}

/*!
 * \fn Light::UniqueID::UniqueID(const QJsonValue json)
 *
 * Constructs a UniqueID object by parsing JSON specified
 * by \a json.
 *
 */
Light::UniqueID::UniqueID(const QJsonValue json)
{
    m_uniqueID = json.toString();
}

/*!
 * \fn QString Light::UniqueID::getUniqueID() const
 *
 * Returns unique ID as a \e QString.
 *
 */
QString Light::UniqueID::getUniqueID() const
{
    return m_uniqueID;
}

/*!
 * \fn void Light::UniqueID::setUniqueID(const QString uniqueID)
 *
 * Sets unique ID as specified by \a uniqueID.
 *
 */
void Light::UniqueID::setUniqueID(const QString uniqueID)
{
    m_uniqueID = uniqueID;
}

// ---------- SOFTWARE VERSION ----------

/*!
 * \class Light::SoftwareVersion
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief Describes the software version of a \l HueLight.
 *
 */

/*!
 * \fn Light::SoftwareVersion::SoftwareVersion()
 *
 * Constructs an empty SoftwareVersion object.
 *
 */
Light::SoftwareVersion::SoftwareVersion()
    : m_softwareVersion("")
{

}

/*!
 * \fn Light::SoftwareVersion::SoftwareVersion(const QJsonValue json)
 *
 * Constructs a SoftwareVersion object by parsing JSON specified
 * by \a json.
 *
 */
Light::SoftwareVersion::SoftwareVersion(const QJsonValue json)
{
    m_softwareVersion = json.toString();
}

/*!
 * \fn QString Light::SoftwareVersion::getSoftwareVersion() const
 *
 * Returns software version as a \e QString.
 *
 */
QString Light::SoftwareVersion::getSoftwareVersion() const
{
    return m_softwareVersion;
}

/*!
 * \fn void Light::SoftwareVersion::setSoftwareVersion(const QString softwareVersion)
 *
 * Sets software version as specified by \a softwareVersion.
 *
 */
void Light::SoftwareVersion::setSoftwareVersion(const QString softwareVersion)
{
    m_softwareVersion = softwareVersion;
}

// ---------- SOFTWARE UPDATE ----------

/*!
 * \class Light::SoftwareUpdate
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief Describes the software update of a \l HueLight.
 *
 */

/*!
 * \fn Light::SoftwareUpdate::SoftwareUpdate()
 *
 * Constructs an empty SoftwareUpdate object.
 *
 */
Light::SoftwareUpdate::SoftwareUpdate()
    : m_state("")
    , m_lastInstall("")
{

}

/*!
 * \fn Light::SoftwareUpdate::SoftwareUpdate(const QJsonValue json)
 *
 * Constructs a SoftwareUpdate object by parsing JSON specified
 * by \a json.
 *
 */
Light::SoftwareUpdate::SoftwareUpdate(const QJsonValue json)
{
    QJsonObject softwareUpdateJson = json.toObject();

    bool jsonIsValid =
            softwareUpdateJson.contains("state")        &
            softwareUpdateJson.contains("lastinstall")  ;

    if (jsonIsValid) {
        m_state         = softwareUpdateJson["state"].toString();
        m_lastInstall   = softwareUpdateJson["lastinstall"].toString();
    }
    else {
        Light::SoftwareUpdate();
    }
}

/*!
 * \fn QString Light::SoftwareUpdate::getState() const
 *
 * Returns state as a \e QString.
 *
 */
QString Light::SoftwareUpdate::getState() const
{
    return m_state;
}

/*!
 * \fn QString Light::SoftwareUpdate::getLastInstall() const
 *
 * Returns last install as a \e QString.
 *
 */
QString Light::SoftwareUpdate::getLastInstall() const
{
    return m_lastInstall;
}

/*!
 * \fn void Light::SoftwareUpdate::setState(const QString state)
 *
 * Sets state as specified by \a state.
 *
 */
void Light::SoftwareUpdate::setState(const QString state)
{
    m_state = state;
}

/*!
 * \fn void Light::SoftwareUpdate::setLastInstall(const QString lastInstall)
 *
 * Sets last install as specified by \a lastInstall.
 *
 */
void Light::SoftwareUpdate::setLastInstall(const QString lastInstall)
{
    m_lastInstall = lastInstall;
}

// ---------- SOFTWARE CONFIG ID ----------

/*!
 * \class Light::SoftwareConfigID
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief Describes the software config ID of a \l HueLight.
 *
 */

/*!
 * \fn Light::SoftwareConfigID::SoftwareConfigID()
 *
 * Constructs an empty SoftwareConfigID object.
 *
 */
Light::SoftwareConfigID::SoftwareConfigID()
    : m_softwareConfigID("")
{

}

/*!
 * \fn Light::SoftwareConfigID::SoftwareConfigID(const QJsonValue json)
 *
 * Constructs a SoftwareConfigID object by parsing JSON specified
 * by \a json.
 *
 */
Light::SoftwareConfigID::SoftwareConfigID(const QJsonValue json)
{
    m_softwareConfigID = json.toString();
}

/*!
 * \fn QString Light::SoftwareConfigID::getSoftwareConfigID() const
 *
 * Returns software config ID as a \e QString.
 *
 */
QString Light::SoftwareConfigID::getSoftwareConfigID() const
{
    return m_softwareConfigID;
}

/*!
 * \fn void Light::SoftwareConfigID::setSoftwareConfigID(const QString softwareConfigID)
 *
 * Sets software config ID as specified by \a softwareConfigID.
 *
 */
void Light::SoftwareConfigID::setSoftwareConfigID(const QString softwareConfigID)
{
    m_softwareConfigID = softwareConfigID;
}

// ---------- PRODUCT NAME ----------

/*!
 * \class Light::ProductName
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief Describes the product name of a \l HueLight.
 *
 */

/*!
 * \fn Light::ProductName::ProductName()
 *
 * Constructs an empty ProductName object.
 *
 */
Light::ProductName::ProductName()
    : m_productName("")
{

}

/*!
 * \fn Light::ProductName::ProductName(const QJsonValue json)
 *
 * Constructs a ProductName object by parsing JSON specified
 * by \a json.
 *
 */
Light::ProductName::ProductName(const QJsonValue json)
{
    m_productName = json.toString();
}

/*!
 * \fn QString Light::ProductName::getProductName() const
 *
 * Returns product name as a \e QString.
 *
 */
QString Light::ProductName::getProductName() const
{
    return m_productName;
}

/*!
 * \fn void Light::ProductName::setProductName(const QString productName)
 *
 * Sets product name as specified by \a productName.
 *
 */
void Light::ProductName::setProductName(const QString productName)
{
    m_productName = productName;
}

// ---------- MANUFACTURER ----------

/*!
 * \class Light::Manufacturer
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief Describes the manufacturer of a \l HueLight.
 *
 */

/*!
 * \fn Light::Manufacturer::Manufacturer()
 *
 * Constructs an empty Manufacturer object.
 *
 */
Light::Manufacturer::Manufacturer()
    : m_manufacturer("")
{

}

/*!
 * \fn Light::Manufacturer::Manufacturer(const QJsonValue json)
 *
 * Constructs a Manufacturer object by parsing JSON specified
 * by \a json.
 *
 */
Light::Manufacturer::Manufacturer(const QJsonValue json)
{
    m_manufacturer = json.toString();
}

/*!
 * \fn QString Light::Manufacturer::getManufacturer() const
 *
 * Returns manufacturer as a \e QString.
 *
 */
QString Light::Manufacturer::getManufacturer() const
{
    return m_manufacturer;
}

/*!
 * \fn void Light::Manufacturer::setManufacturer(const QString manufacturer)
 *
 * Sets manufacturer as specified by \a manufacturer.
 *
 */
void Light::Manufacturer::setManufacturer(const QString manufacturer)
{
    m_manufacturer = manufacturer;
}

// ---------- PRODUCT ID ----------

/*!
 * \class Light::ProductID
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief Describes the product ID of a \l HueLight.
 *
 */

/*!
 * \fn Light::ProductID::ProductID()
 *
 * Constructs an empty ProductID object.
 *
 */
Light::ProductID::ProductID()
    : m_productID("")
{

}

/*!
 * \fn Light::ProductID::ProductID(const QJsonValue json)
 *
 * Constructs a ProductID object by parsing JSON specified
 * by \a json.
 *
 */
Light::ProductID::ProductID(const QJsonValue json)
{
    m_productID = json.toString();
}

/*!
 * \fn QString Light::ProductID::getProductID() const
 *
 * Returns product ID as a \e QString.
 *
 */
QString Light::ProductID::getProductID() const
{
    return m_productID;
}

/*!
 * \fn void Light::ProductID::setProductID(const QString productID)
 *
 * Sets product ID as specified by \a productID.
 *
 */
void Light::ProductID::setProductID(const QString productID)
{
    m_productID = productID;
}

// ---------- Config ----------
/*!
 * \class Light::Config
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief Describes the config of a \l HueLight.
 *
 */

/*!
 * \class Light::Config::Startup
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief Describes the startup in \l Config of a \l HueLight.
 *
 */

/*!
 * \fn Light::Config::Startup::Startup()
 *
 * Constructs an empty Startup object.
 *
 */
Light::Config::Startup::Startup()
    : m_mode("")
    , m_configured(false)
{

}

/*!
 * \fn Light::Config::Startup::Startup(const QJsonValue json)
 *
 * Constructs a Startup object by parsing JSON specified
 * by \a json.
 *
 */
Light::Config::Startup::Startup(const QJsonValue json)
{
    QJsonObject startupJson = json.toObject();

    bool jsonIsValid =
            startupJson.contains("mode")        &
            startupJson.contains("configured")  ;

    if (jsonIsValid) {
        m_mode          = startupJson["mode"].toString();
        m_configured    = startupJson["configured"].toBool();
    }
    else {
        Light::Config::Startup();
    }
}

/*!
 * \fn QString Light::Config::Startup::getMode() const
 *
 * Returns mode as a \e QString.
 *
 */
QString Light::Config::Startup::getMode() const
{
    return m_mode;
}

/*!
 * \fn bool Light::Config::Startup::getConfigured() const
 *
 * Returns \c true if configured.
 *
 */
bool Light::Config::Startup::getConfigured() const
{
    return m_configured;
}

/*!
 * \fn void Light::Config::Startup::setMode(const QString mode)
 *
 * Sets mode as specified by \a mode.
 *
 */
void Light::Config::Startup::setMode(const QString mode)
{
    m_mode = mode;
}

/*!
 * \fn void Light::Config::Startup::setConfigured(bool configured)
 *
 * Sets configured as specified by \a configured.
 *
 */
void Light::Config::Startup::setConfigured(bool configured)
{
    m_configured = configured;
}

/*!
 * \fn Light::Config::Config()
 *
 * Constructs an empty Config object.
 *
 */
Light::Config::Config()
    : m_archetype("")
    , m_function("")
    , m_direction("")
    , m_startup()
{

}

/*!
 * \fn Light::Config::Config(const QJsonValue json)
 *
 * Constructs a Config object by parsing JSON specified
 * by \a json.
 *
 */
Light::Config::Config(const QJsonValue json)
{
    QJsonObject configJson = json.toObject();

    bool jsonIsValid =
            configJson.contains("archetype")    &
            configJson.contains("function")     &
            configJson.contains("direction")    &
            configJson.contains("startup")      ;

    if (jsonIsValid) {
        m_archetype     = configJson["archetype"].toString();
        m_function      = configJson["function"].toString();
        m_direction     = configJson["direction"].toString();
        m_startup       = Light::Config::Startup(configJson["startup"]);
    }
    else {
        Light::Config();
    }
}

/*!
 * \fn QString Light::Config::getArchetype() const
 *
 * Returns archetype as a \e QString.
 *
 */
QString Light::Config::getArchetype() const
{
    return m_archetype;
}

/*!
 * \fn QString Light::Config::getFunction() const
 *
 * Returns function as a \e QString.
 *
 */
QString Light::Config::getFunction() const
{
    return m_function;
}

/*!
 * \fn QString Light::Config::getDirection() const
 *
 * Returns direction as a \e QString.
 *
 */
QString Light::Config::getDirection() const
{
    return m_direction;
}

/*!
 * \fn Light::Config::Startup Light::Config::getStartup() const
 *
 * Returns startup as a \l Startup object.
 *
 */
Light::Config::Startup Light::Config::getStartup() const
{
    return m_startup;
}

/*!
 * \fn void Light::Config::setArchetype(const QString archetype)
 *
 * Sets archetype as specified by \a archetype.
 *
 */
void Light::Config::setArchetype(const QString archetype)
{
    m_archetype = archetype;
}

/*!
 * \fn void Light::Config::setFunction(const QString function)
 *
 * Sets function as specified by \a function.
 *
 */
void Light::Config::setFunction(const QString function)
{
    m_function = function;
}

/*!
 * \fn void Light::Config::setDirection(const QString direction)
 *
 * Sets direction as specified by \a direction.
 *
 */
void Light::Config::setDirection(const QString direction)
{
    m_direction = direction;
}

/*!
 * \fn void Light::Config::setStartup(const Startup startup)
 *
 * Sets startup as specified by \a startup.
 *
 */
void Light::Config::setStartup(const Startup startup)
{
    m_startup = startup;
}

/* =====================================
 * =============== GROUP ===============
 * =====================================
 *
 */

// ---------- ACTION ----------
Group::Action::Action()
    : m_on(false)
    , m_brightness(0)
    , m_hue(0)
    , m_saturation(0)
    , m_colorTemp(0)
    , m_xValue(0)
    , m_yValue(0)
    , m_effect("")
    , m_alert("")
    , m_colorMode("")
{

}

Group::Action::Action(const QJsonValue json)
{
    QJsonObject actionJson = json.toObject();

    bool jsonIsValid =
            actionJson.contains("on")        &
            actionJson.contains("bri")       &
            actionJson.contains("hue")       &
            actionJson.contains("sat")       &
            actionJson.contains("ct")        &
            actionJson.contains("xy")        &
            actionJson.contains("effect")    &
            actionJson.contains("alert")     &
            actionJson.contains("colormode") ;

    if (jsonIsValid) {
        m_on            = actionJson["on"].toBool();
        m_brightness    = actionJson["bri"].toInt();
        m_hue           = actionJson["hue"].toInt();
        m_saturation    = actionJson["sat"].toInt();
        m_colorTemp     = actionJson["ct"].toInt();
        m_xValue        = actionJson["xy"].toArray()[0].toDouble();
        m_yValue        = actionJson["xy"].toArray()[1].toDouble();
        m_effect        = actionJson["effect"].toString();
        m_alert         = actionJson["alert"].toString();
        m_colorMode     = actionJson["colormode"].toString();
    }
    else {
        Group::Action();
    }
}

bool Group::Action::isOn() const
{
    return m_on;
}

int Group::Action::getBrightness() const
{
    return m_brightness;
}

int Group::Action::getHue() const
{
    return m_hue;
}

int Group::Action::getSaturation() const
{
    return m_saturation;
}

int Group::Action::getColorTemp() const
{
    return m_colorTemp;
}

double Group::Action::getXValue() const
{
    return m_xValue;
}

double Group::Action::getYValue() const
{
    return m_yValue;
}

QString Group::Action::getEffect() const
{
    return m_effect;
}

QString Group::Action::getAlert() const
{
    return m_alert;
}

QString Group::Action::getColorMode() const
{
    return m_colorMode;
}

void Group::Action::setOn(const bool on)
{
    m_on = on;
}

void Group::Action::setBrightness(const int brightness)
{
    m_brightness = brightness;
}

void Group::Action::setHue(const int hue)
{
    m_hue = hue;
}

void Group::Action::setSaturation(const int saturation)
{
    m_saturation = saturation;
}

void Group::Action::setColorTemp(const int colorTemp)
{
    m_colorTemp = colorTemp;
}

void Group::Action::setXValue(const double xValue)
{
    m_xValue = xValue;
}

void Group::Action::setYValue(const double yValue)
{
    m_yValue = yValue;
}

void Group::Action::setEffect(const QString effect)
{
    m_effect = effect;
}

void Group::Action::setAlert(const QString alert)
{
    m_alert = alert;
}

void Group::Action::setColorMode(const QString colorMode)
{
    m_colorMode = colorMode;
}

// ---------- LIGHTS ----------
Group::Lights::Lights()
    : m_lights()
{

}

Group::Lights::Lights(const QJsonValue json)
    : m_lights()
{
    QJsonArray lightsJson = json.toArray();
    for (auto iter = lightsJson.begin(); iter != lightsJson.end(); ++iter) {
        m_lights.append(iter->toString());
    }
}

QList<QString> Group::Lights::getLights() const
{
    return m_lights;
}

void Group::Lights::setLights(const QList<QString> lights)
{
    m_lights = lights;
}

// ---------- SENSORS ----------
Group::Sensors::Sensors()
    : m_sensors()
{

}

Group::Sensors::Sensors(const QJsonValue json) :
    m_sensors()
{
    QJsonArray sensorsJson = json.toArray();
    for (auto iter = sensorsJson.begin(); iter != sensorsJson.end(); ++iter) {
        m_sensors.append(iter->toString());
    }
}

QList<QString> Group::Sensors::getSensors() const
{
    return m_sensors;
}

void Group::Sensors::setSensors(const QList<QString> sensors)
{
    m_sensors = sensors;
}

// ---------- STATE ----------
Group::State::State()
    : m_allOn(false)
    , m_anyOn(false)
{

}

Group::State::State(const QJsonValue json)
{
    QJsonObject stateJson = json.toObject();

    if (stateJson.contains("all_on") && stateJson.contains("any_on")) {
        m_allOn = stateJson["all_on"].toBool();
        m_anyOn = stateJson["any_on"].toBool();
    }
    else {
        Group::State();
    }
}

bool Group::State::getAllOn() const
{
    return m_allOn;
}

bool Group::State::getAnyOn() const
{
    return m_anyOn;
}

void Group::State::setAllOn(const bool allOn)
{
    m_allOn = allOn;
}

void Group::State::setAnyOn(const bool anyOn)
{
    m_anyOn = anyOn;
}

// ---------- NAME ----------
Group::Name::Name()
    : m_name("")
{

}

Group::Name::Name(const QJsonValue json)
{
    m_name = json.toString();
}

QString Group::Name::getName() const
{
    return m_name;
}

void Group::Name::setName(const QString name)
{
    m_name = name;
}

// ---------- TYPE ----------
Group::Type::Type()
    : m_type("")
{

}

Group::Type::Type(const QJsonValue json)
{
    m_type = json.toString();
}

QString Group::Type::getType() const
{
    return m_type;
}

void Group::Type::setType(const QString type)
{
    m_type = type;
}

// ---------- GROUP CLASS ----------
Group::GroupClass::GroupClass()
    : m_groupClass("")
{

}

Group::GroupClass::GroupClass(const QJsonValue json)
{
    m_groupClass = json.toString();
}

QString Group::GroupClass::getGroupClass() const
{
    return m_groupClass;
}

void Group::GroupClass::setGroupClass(const QString groupClass)
{
    m_groupClass = groupClass;
}

// ---------- Recycle ----------
Group::Recycle::Recycle()
    : m_recycle(false)
{

}

Group::Recycle::Recycle(const QJsonValue json)
{
    m_recycle = json.toBool();
}

bool Group::Recycle::getRecycle() const
{
    return m_recycle;
}

void Group::Recycle::setRecycle(const bool recycle)
{
    m_recycle = recycle;
}

