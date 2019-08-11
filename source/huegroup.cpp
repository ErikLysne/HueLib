#include "huegroup.h"

#include "huebridge.h"
#include "huerequest.h"
#include "huereply.h"
#include "huelight.h"


/*!
 * \class HueGroup
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief The HueGroup class is used to interface with a group of Hue lights.
 *
 * A HueGroup object mirrors the state of a Hue group registered on the network.
 * HueGroup objects cannot be constructed directly because the constructor of HueGroup
 * is private. Instead, a list of HueGroup objects (a \l HueGroupList) is returned
 * by the static function \l discoverGroups(). A \l HueBridge object must be constructed
 * before any HueLight objects can be constructed.
 *
 * The following minimum working example turns off all groups on the network, then turns
 * on the group named "Living Room" and enables the color loop effect.
 *
 * \code
 *  // Create HueBridge
 *  HueBridge* bridge = new HueBridge("10.0.1.14");
 *
 *  // Link to bridge
 *  bridge->link();
 *
 *  // Discover groups on the network
 *  HueGroupList* groups = new HueGroupList(HueGroup::discoverGroups(bridge));
 *
 *  // Turn off all groups
 *  for (auto group : *groups)
 *      group->turnOff();
 *
 *  // Turn on light 1 and set effect to color loop
 *  groups->fetch("Living Room")->turnOn();
 *  groups->fetch("Living Room")->turnOn();
 * \endcode
 *
 * As can be seen from the example above, groups can be changed by directly calling
 * \e set functions on the HueGroup objects. Below is a list of \e set functions that
 * work with HueGroup objects.
 *
 * \table
 * \header
 *  \li Function
 *  \li Description
 *  \li Range
 * \row
 *  \li \l turnOn(const bool on = true)
 *  \li Turns the group on.
 *  \li \c true or \c false
 *
 * \row
 *  \li \l turnOff(const bool off = true)
 *  \li Turns the group off.
 *  \li \c true or \c false
 *
 * \row
 *  \li \l setHue(const int hue)
 *  \li Sets the hue of the group.
 *  \li 0 - 65535
 *
 * \row
 *  \li \l setSaturation(const int saturation)
 *  \li Sets the saturation of the group.
 *  \li 0 - 254
 *
 * \row
 *  \li \l setBrightness(const int brightness)
 *  \li Sets the brightness of the group.
 *  \li 1 - 254
 *
 * \row
 *  \li \l setColorTemp(const int colorTemp)
 *  \li Sets the color temperature of the group.
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
 *  \li {1, 3} Sets an alert on the group.
 *  \li \l NoAlert
 *  \row
 *  \li \l BreatheSingle
 *  \row
 *  \li \l Breathe15Sec
 *
 * \row
 *  \li {1, 2} \l setEffect(const HueEffect effect);
 *  \li {1, 2} Sets an effect on the group.
 *  \li \l NoEffect
 *  \row
 *  \li \l ColorLoop
 *
 * \endtable
 *
 * Data can be retreved from HueGroup by calling the functions in the table below.
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
 *  \li {1, 11} \l Group::Action action()
 *  \li {1, 11} Describes the action perfomed on a group (similar to \l HueLight::state()
 *  \li \c bool \l Group::Action::isOn()
 *  \row
 *  \li \c int \l Group::Action::getBrightness()
 *  \row
 *  \li \c int \l Group::Action::getHue()
 *  \row
 *  \li \c int \l Group::Action::getSaturation()
 *  \row
 *  \li \c int \l Group::Action::getColorTemp()
 *  \row
 *  \li \c double \l Group::Action::getXValue()
 *  \row
 *  \li \c double \l Group::Action::getYValue()
 *  \row
 *  \li \c QString \l Group::Action::getEffect()
 *  \row
 *  \li \c QString \l Group::Action::getAlert()
 *  \row
 *  \li \c QString \l Group::Action::getColorMode()
 *  \row
 *  \li
 *
 * \row
 *  \li {1, 1} \l Group::Lights lights()
 *  \li {1, 1} List of lights associated with the group (by ID).
 *  \li \c QList<QString> \l Group::Lights::getLights()
 *
 * \row
 *  \li {1, 1} \l Group::Sensors sensors()
 *  \li {1, 1} List of sensors associated with the group.
 *  \li \c QList<QString> \l Group::Sensors::getSensors()
 *
 * \row
 *  \li {1, 3} \l Group::State state()
 *  \li {1, 3} State of the group.
 *  \li \c bool \l Group::State::getAllOn()
 *  \row
 *  \li \c bool \l Group::State::getAnyOn()
 *  \row
 *  \li
 *
 * \row
 *  \li {1, 1} \l Group::Name name()
 *  \li {1, 1} Name of the group.
 *  \li \c QString \l Group::Name::getName()
 *
 * \row
 *  \li {1, 1} \l Group::Type type()
 *  \li {1, 1} Describes the type of group.
 *  \li \c QString \l Group::Type::getType()
 *
 * \row
 *  \li {1, 1} \l Group::GroupClass groupClass()
 *  \li {1, 1} Describes the class of the group.
 *  \li \c QString \l Group::GroupClass::getGroupClass()
 *
 * \row
 *  \li {1, 1} \l Group::Recycle recycle()
 *  \li {1, 1} Determines if the group will recycle (be deleted
 *             when not referenced).
 *  \li \c QString \l Group::Recycle::getRecycle()
 *  \row
 *
 * \endtable
 *
 */

/*!
 * \fn HueGroup::HueGroup()
 *
 * Creates an empty HueGroup.
 *
 * \note should not be called explicitly.
 *
 * \sa discoverGroups()
 *
 */
HueGroup::HueGroup()
    : HueAbstractObject(nullptr)
    , m_ID()
    , m_action()
    , m_lights()
    , m_sensors()
    , m_state()
    , m_name()
    , m_type()
    , m_groupClass()
    , m_recycle()
    , m_validConstructor(false)
{

}

/*!
 * \fn HueGroup(HueBridge* bridge)
 *
 * Creates an empty HueGroup with a \l HueBridge reference
 * provided by \a bridge.
 *
 * \note should not be called explicitly.
 *
 * \sa discoverGroups()
 *
 */
HueGroup::HueGroup(HueBridge* bridge)
    : HueAbstractObject(bridge)
    , m_ID()
    , m_action()
    , m_lights()
    , m_sensors()
    , m_state()
    , m_name()
    , m_type()
    , m_groupClass()
    , m_recycle()
    , m_validConstructor(false)
{

}

HueGroup::HueGroup(HueBridge* bridge,
                   int ID,
                   Group::Action action, Group::Lights lights,
                   Group::Sensors sensors, Group::State state,
                   Group::Name name, Group::Type type,
                   Group::GroupClass groupClass, Group::Recycle recycle)
    : HueAbstractObject(bridge)
    , m_ID(ID)
    , m_action(action)
    , m_lights(lights)
    , m_sensors(sensors)
    , m_state(state)
    , m_name(name)
    , m_type(type)
    , m_groupClass(groupClass)
    , m_recycle(recycle)
    , m_validConstructor(true)
{

}

HueGroup::HueGroup(const HueGroup& rhs)
    : HueAbstractObject(rhs.getBridge())
    , m_ID(rhs.m_ID)
    , m_action(rhs.m_action)
    , m_lights(rhs.m_lights)
    , m_sensors(rhs.m_sensors)
    , m_state(rhs.m_state)
    , m_name(rhs.m_name)
    , m_type(rhs.m_type)
    , m_groupClass(rhs.m_groupClass)
    , m_recycle(rhs.m_recycle)
    , m_validConstructor(rhs.m_validConstructor)
{

}

HueGroup HueGroup::operator=(const HueGroup &rhs)
{
    if (&rhs == this)
        return *this;

    this->setBridge(rhs.getBridge());
    this->setParent(rhs.parent());
    m_ID = rhs.m_ID;
    m_action = rhs.m_action;
    m_lights = rhs.m_lights;
    m_sensors = rhs.m_sensors;
    m_state = rhs.m_state;
    m_name = rhs.m_name;
    m_type = rhs.m_type;
    m_groupClass = rhs.m_groupClass;
    m_recycle = rhs.m_recycle;
    m_validConstructor = rhs.m_validConstructor;

    return *this;
}

/*!
 * \fn HueGroupList HueGroup::discoverGroups(HueBridge *bridge)
 *
 * Returns a \l HueGroupList of groups registered on the bridge.
 * This function is static and is used to generate HueGroup objects
 * using the connection to \l HueBridge specified by \a bridge.
 *
 */
HueGroupList HueGroup::discoverGroups(HueBridge *bridge)
{
    if (bridge == nullptr)
        return HueGroupList();

    std::shared_ptr<GroupVector> groups = std::make_shared<GroupVector>();

    HueRequest request("groups", QJsonObject(), HueRequest::Get);
    HueReply reply = bridge->sendRequest(request, nullptr);

    if (!reply.isValid() || reply.timedOut() || reply.containsError()) {
        qDebug().noquote() << reply;
        return HueGroupList();
    }

    QJsonObject json = reply.getJson();
    auto parsedJsonWithIDs = HueAbstractObject::parseJson(json);

    if (parsedJsonWithIDs.isEmpty())
        return HueGroupList();

    for (auto iter = parsedJsonWithIDs.begin(); iter != parsedJsonWithIDs.end(); ++iter) {
        int ID = iter.key();
        QJsonObject json = iter.value();

        std::shared_ptr<HueGroup> group = std::make_shared<HueGroup>(bridge);

        if (HueGroup::constructHueGroup(ID, json, group))
            groups.get()->push_back(std::move(group));

    }

    return HueGroupList(std::move(groups));
}

/*!
 * \fn HueLightList HueGroup::getLights(const HueLightList& lights) const
 *
 * Returns a \l HueLightList of \l HueLight objects associated with the
 * HueGroup. \l HueLight::discoverLights() must be called prior to calling
 * this function, and the \l HueLightList returned must be passed specified by
 * \a lights.
 *
 * \code
 *  // Create HueBridge
 *  HueBridge* bridge = new HueBridge("10.0.1.14");
 *
 *  // Link to bridge
 *  bridge->link();
 *
 *  // Discover lights and groups
 *  HueLightList* lights = new HueLightList(HueLight::discoverLights(bridge));
 *  HueGroupList* groups = new HueGroupList(HueGroup::discoverGroups(bridge));
 *
 *  //  Get a raw pointer to the "Living Room" group
 *  HueGroup* livingRoom = groups->fetchRaw("Living Room");
 *
 *  // Get the lights in "Living Room"
 *  HueLightList* livingRoomLights = new HueLightList(livingRoom->getLights(lights));
 *
 *  // Turn off all groups
 *  for (auto group : *groups)
 *      group->turnOff();
 *
 *  int rbgHue[3] = {
 *          65535,  // red
 *          21845,  // green
 *          43690   // blue
 *  };
 *
 *  // Control lights in "Living Room" individually
 *  int index = 0;
 *  for (auto light : *livingRoomLights)
 *          light->setHue(rbgHue[index++ % 3]);
 * \endcode
 *
 * \sa HueLight::discoverLights(), HueGroup::discoverGroups()
 *
 */
HueLightList HueGroup::getLights(const HueLightList& lights) const
{
    LightVector foundLights;
    for (auto lightIDs : m_lights.getLights()) {
        int lightID = lightIDs.toInt();
        foundLights.push_back(lights.fetch(lightID));
    }

    return HueLightList(std::make_shared<LightVector>(foundLights));
}

/*!
 * \fn Group::Action HueGroup::action() const
 *
 * Returns a \l Group::Action object.
 *
 */
Group::Action HueGroup::action() const
{
    return m_action;
}

/*!
 * \fn Group::Lights HueGroup::lights() const
 *
 * Returns a \l Group::Lights object.
 *
 */
Group::Lights HueGroup::lights() const
{
    return m_lights;
}

/*!
 * \fn Group::Sensors HueGroup::sensors() const
 *
 * Returns a \l Group::Sensors object.
 *
 */
Group::Sensors HueGroup::sensors() const
{
    return m_sensors;
}

/*!
 * \fn Group::State HueGroup::state() const
 *
 * Returns a \l Group::State object.
 *
 */
Group::State HueGroup::state() const
{
    return m_state;
}

/*!
 * \fn Group::Name HueGroup::name() const
 *
 * Returns a \l Group::Name object.
 *
 */
Group::Name HueGroup::name() const
{
    return m_name;
}

/*!
 * \fn Group::Type HueGroup::type() const
 *
 * Returns a \l Group::Type object.
 *
 */
Group::Type HueGroup::type() const
{
    return m_type;
}

/*!
 * \fn Group::GroupClass HueGroup::groupClass() const
 *
 * Returns a \l Group::GroupClass object.
 *
 */
Group::GroupClass HueGroup::groupClass() const
{
    return m_groupClass;
}

/*!
 * \fn Group::Recycle HueGroup::recycle() const
 *
 * Returns a \l Group::Recycle object.
 *
 */
Group::Recycle HueGroup::recycle() const
{
    return m_recycle;
}


/*!
 * \fn bool HueGroup::hasValidConstructor() const
 *
 * Returns \c true if HueGroup was constructed properly.
 *
 * \sa isValid()
 *
 */
bool HueGroup::hasValidConstructor() const
{
    return m_validConstructor;
}

/*!
 * \fn bool HueGroup::isValid() const
 *
 * Equivalent to \l hasValidConstructor()
 *
 */
bool HueGroup::isValid() const
{
    return hasValidConstructor();
}

/*!
 * \fn int HueGroup::ID() const
 *
 * Returns an \c int with the ID of the group.
 *
 */
int HueGroup::ID() const
{
    return m_ID;
}

/*!
 * \fn bool HueGroup::synchronize()
 *
 * Updates HueGroup to the lastest state available on \l HueBridge.
 *
 * \note only queries an individual HueGroup from the bridge.
 * If all groups needs to be updated, it can be faster to delete the
 * \l HueGroupList and call \l discoverGroups() again.
 *
 * \sa discoverGroups()
 *
 */
bool HueGroup::synchronize()
{
    HueRequest syncRequest = makeGetRequest();
    HueReply syncReply;

    bool replyValid = sendRequest(syncRequest, syncReply);

    if (replyValid) {
        QJsonObject json = syncReply.getJson();
        std::shared_ptr<HueGroup> synchronizedGroup = std::make_shared<HueGroup>(getBridge());

        if (constructHueGroup(m_ID, json, synchronizedGroup)) {
            if (synchronizedGroup->hasValidConstructor()) {
                *this = *synchronizedGroup.get();

                emit synchronized();
                return true;
            }
        }
    }

    return false;
}

bool HueGroup::constructHueGroup(int ID, QJsonObject json, std::shared_ptr<HueGroup>& group)
{
    bool jsonIsValid =
            json.contains("name")       &
            json.contains("lights")     &
            json.contains("sensors")    &
            json.contains("type")       &
            json.contains("state")      &
            json.contains("recycle")    &
            json.contains("class")      &
            json.contains("action")     ;

    if(!jsonIsValid)
        return false;

    Group::Action action(json["action"]);
    Group::Lights lights(json["lights"]);
    Group::Sensors sensors(json["sensors"]);
    Group::State state(json["state"]);
    Group::Name name(json["name"]);
    Group::Type type(json["type"]);
    Group::GroupClass groupClass(json["class"]);
    Group::Recycle recycle(json["recycle"]);

    HueGroup* newGroup = new HueGroup(group->getBridge(),
                                      ID,
                                      action,
                                      lights,
                                      sensors,
                                      state,
                                      name,
                                      type,
                                      groupClass,
                                      recycle);

    group.reset(newGroup);

    return true;
}

HueRequest HueGroup::makePutRequest(QJsonObject json)
{
    QString urlPath = "groups/" + QString::number(m_ID) + "/action";
    HueRequest::Method method = HueRequest::Put;

    return HueRequest(urlPath, json, method);
}

HueRequest HueGroup::makeGetRequest()
{
    QString urlPath = "groups/" + QString::number(m_ID);
    HueRequest::Method method = HueRequest::Get;

    return HueRequest(urlPath, QJsonObject(), method);
}

void HueGroup::updateOn(const bool on)
{
    m_action.setOn(on);
    emit valueUpdated();
}

void HueGroup::updateHue(const int hue)
{
    m_action.setHue(hue);
    emit valueUpdated();
}

void HueGroup::updateSaturation(const int saturation)
{
    m_action.setSaturation(saturation);
    emit valueUpdated();
}

void HueGroup::updateBrightness(const int brightness)
{
    m_action.setBrightness(brightness);
    emit valueUpdated();
}

void HueGroup::updateColorTemp(const int colorTemp)
{
    m_action.setColorTemp(colorTemp);
    emit valueUpdated();
}

void HueGroup::updateXY(const double x, const double y)
{
    m_action.setXValue(x);
    m_action.setYValue(y);
    emit valueUpdated();
}

void HueGroup::updateAlert(const HueAlert alert)
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
    m_action.setAlert(alertString);
    emit valueUpdated();
}

void HueGroup::updateEffect(const HueEffect effect)
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
    m_action.setEffect(effectString);
    emit valueUpdated();
}



