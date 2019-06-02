#include "huetypes.h"
#include <QJsonArray>

/* =====================================
 * =============== LIGHT ===============
 * =====================================
 *
 */

// ---------- STATE ----------

Light::State::State() :
    m_on(false),
    m_reachable(false),
    m_brightness(0),
    m_hue(0),
    m_saturation(0),
    m_colorTemp(0),
    m_xValue(0),
    m_yValue(0),
    m_effect(""),
    m_alert(""),
    m_colorMode(""),
    m_mode("")
{

}

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

bool Light::State::isOn() const
{
    return m_on;
}

bool Light::State::isReachable() const
{
    return m_reachable;
}

int Light::State::getBrightness() const
{
    return m_brightness;
}

int Light::State::getHue() const
{
    return m_hue;
}

int Light::State::getSaturation() const
{
    return m_saturation;
}

int Light::State::getColorTemp() const
{
    return m_colorTemp;
}

double Light::State::getXValue() const
{
    return m_xValue;
}

double Light::State::getYValue() const
{
    return m_yValue;
}

QString Light::State::getEffect() const
{
    return m_effect;
}

QString Light::State::getAlert() const
{
    return m_alert;
}

QString Light::State::getColorMode() const
{
    return m_colorMode;
}

QString Light::State::getMode() const
{
    return m_mode;
}

void Light::State::setOn(const bool on)
{
    m_on = on;
}

void Light::State::setReachable(const bool reachable)
{
    m_reachable = reachable;
}

void Light::State::setBrightness(const int brightness)
{
    m_brightness = brightness;
}

void Light::State::setHue(const int hue)
{
    m_hue = hue;
}

void Light::State::setSaturation(const int saturation)
{
    m_saturation = saturation;
}

void Light::State::setEffect(const QString effect)
{
    m_effect = effect;
}

void Light::State::setAlert(const QString alert)
{
    m_alert = alert;
}

void Light::State::setColorMode(const QString colorMode)
{
    m_colorMode = colorMode;
}

void Light::State::setMode(const QString mode)
{
    m_mode = mode;
}

// ---------- TYPE ----------
Light::Type::Type() :
    m_type("")
{

}

Light::Type::Type(const QJsonValue json)
{
    m_type = json.toString();
}

QString Light::Type::getType() const
{
    return m_type;
}

void Light::Type::setType(const QString type)
{
    m_type = type;
}

// ---------- Name ----------
Light::Name::Name() :
    m_name("")
{

}

Light::Name::Name(const QJsonValue json)
{
    m_name = json.toString();
}

QString Light::Name::getName() const
{
    return m_name;
}

void Light::Name::setName(const QString name)
{
    m_name = name;
}

// ---------- Unique ID ----------
Light::UniqueID::UniqueID() :
    m_uniqueID("")
{

}

Light::UniqueID::UniqueID(const QJsonValue json)
{
    json["uniqueid"].toString();
}

QString Light::UniqueID::getUniqueID() const
{
    return m_uniqueID;
}

void Light::UniqueID::setUniqueID(const QString uniqueID)
{
    m_uniqueID = uniqueID;
}

// ---------- SOFTWARE VERSION ----------
Light::SoftwareVersion::SoftwareVersion() :
    m_softwareVersion("")
{

}

Light::SoftwareVersion::SoftwareVersion(const QJsonValue json)
{
    m_softwareVersion = json["swversion"].toString();
}

QString Light::SoftwareVersion::getSoftwareVersion() const
{
    return m_softwareVersion;
}

void Light::SoftwareVersion::setSoftwareVersion(const QString softwareVersion)
{
    m_softwareVersion = softwareVersion;
}

// ---------- SOFTWARE UPDATE ----------
Light::SoftwareUpdate::SoftwareUpdate() :
    m_state(""),
    m_lastInstall("")
{

}

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

QString Light::SoftwareUpdate::getState() const
{
    return m_state;
}

QString Light::SoftwareUpdate::getLastInstall() const
{
    return m_lastInstall;
}

void Light::SoftwareUpdate::setState(const QString state)
{
    m_state = state;
}

void Light::SoftwareUpdate::setLastInstall(const QString lastInstall)
{
    m_lastInstall = lastInstall;
}

// ---------- SOFTWARE CONFIG ID ----------
Light::SoftwareConfigID::SoftwareConfigID() :
    m_softwareConfigID("")
{

}

Light::SoftwareConfigID::SoftwareConfigID(const QJsonValue json)
{
    m_softwareConfigID = json["swconfigid"].toString();
}

QString Light::SoftwareConfigID::getSoftwareConfigID() const
{
    return m_softwareConfigID;
}

void Light::SoftwareConfigID::setSoftwareConfigID(const QString softwareConfigID)
{
    m_softwareConfigID = softwareConfigID;
}

// ---------- PRODUCT NAME ----------
Light::ProductName::ProductName() :
    m_productName("")
{

}

Light::ProductName::ProductName(const QJsonValue json)
{
    m_productName = json.toString();
}

QString Light::ProductName::getProductName() const
{
    return m_productName;
}

void Light::ProductName::setProductName(const QString productName)
{
    m_productName = productName;
}

// ---------- MANUFACTURER ----------
Light::Manufacturer::Manufacturer() :
    m_manufacturer("")
{

}

Light::Manufacturer::Manufacturer(const QJsonValue json)
{
    m_manufacturer = json.toString();
}

QString Light::Manufacturer::getManufacturer() const
{
    return m_manufacturer;
}

void Light::Manufacturer::setManufacturer(const QString manufacturer)
{
    m_manufacturer = manufacturer;
}

// ---------- PRODUCT ID ----------
Light::ProductID::ProductID() :
    m_productID("")
{

}

Light::ProductID::ProductID(const QJsonValue json)
{
    m_productID = json.toString();
}

QString Light::ProductID::getProductID() const
{
    return m_productID;
}

void Light::ProductID::setProductID(const QString productID)
{
    m_productID = productID;
}

/* =====================================
 * =============== GROUP ===============
 * =====================================
 *
 */

// ---------- ACTION ----------
Group::Action::Action() :
    m_on(false),
    m_brightness(0),
    m_hue(0),
    m_saturation(0),
    m_colorTemp(0),
    m_xValue(0),
    m_yValue(0),
    m_effect(""),
    m_alert(""),
    m_colorMode("")
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
Group::Lights::Lights() :
    m_lights()
{

}

Group::Lights::Lights(const QJsonValue json) :
    m_lights()
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
Group::Sensors::Sensors() :
    m_sensors()
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
Group::State::State() :
    m_allOn(false),
    m_anyOn(false)
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
Group::Name::Name() :
    m_name("")
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
Group::Type::Type() :
    m_type("")
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
Group::GroupClass::GroupClass() :
    m_groupClass("")
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
Group::Recycle::Recycle() :
    m_recycle(false)
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

