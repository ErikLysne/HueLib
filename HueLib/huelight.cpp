#include "huelight.h"

#include "huebridge.h"
#include "huerequest.h"
#include "huereply.h"

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
                   Light::ProductID productID)
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
    m_validConstructor = rhs.m_validConstructor;

    return *this;
}

HueLightList HueLight::discoverLights(HueBridge* bridge)
{
    if (bridge == nullptr)
        return HueLightList();

    std::shared_ptr<LightVector> lights = std::make_shared<LightVector>();

    HueRequest request("lights", QJsonObject(), HueRequest::get);
    HueReply reply = bridge->sendRequest(request);

    if (!reply.isValid() || reply.timedOut()) {
        qDebug() << "INVALID HUE REPLY";
        qDebug() << "______________________________";
        qDebug() << "Is valid: " << reply.isValid();
        qDebug() << "Timed out: " << reply.timedOut();
        qDebug() << "HTTP status: " << reply.getHttpStatus();
        qDebug() << "Error type: " << reply.getErrorType();
        qDebug() << "Error address: " << reply.getErrorAddress();
        qDebug() << "Error description: " << reply.getErrorDescription();
        qDebug() << "______________________________";

        return lights;
    }

    QJsonObject json = reply.getJson();
    auto parsedJsonWithIDs = HueAbstractObject::parseJson(json);

    if (parsedJsonWithIDs.isEmpty())
        return lights;

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
                                      productID);

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
    HueRequest::Method method = HueRequest::put;

    return HueRequest(urlPath, json, method);
}

HueRequest HueLight::makeGetRequest()
{
    QString urlPath = "lights/" + QString::number(m_ID);
    HueRequest::Method method = HueRequest::get;

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
