#ifndef HUEGROUP_H
#define HUEGROUP_H

#include "hueabstractobject.h"
#include "huetypes.h"
#include "hueobjectlist.h"

class HueGroup : public HueAbstractObject
{
    Q_OBJECT
public:
    HueGroup();
    HueGroup(HueBridge* bridge);
    HueGroup(const HueGroup& rhs);
    HueGroup operator=(const HueGroup& rhs);

    static HueGroupList discoverGroups(HueBridge* bridge);

    bool hasValidConstructor() const;

    int ID() const;
    Group::Action action() const;
    Group::Lights lights() const;
    Group::Sensors sensors() const;
    Group::State state() const;
    Group::Name name() const;
    Group::Type type() const;
    Group::GroupClass groupClass() const;
    Group::Recycle recycle() const;

    bool synchronize() override;

protected:
    static bool constructHueGroup(int ID, QJsonObject json, HueGroup* group);

    HueGroup(HueBridge* bridge,
             int ID,
             Group::Action action, Group::Lights lights,
             Group::Sensors sensors, Group::State state,
             Group::Name name, Group::Type type,
             Group::GroupClass classObject, Group::Recycle recycle);

    HueRequest makePutRequest(QJsonObject json) override;
    HueRequest makeGetRequest() override;

private:
    int m_ID;
    Group::Action m_action;
    Group::Lights m_lights;
    Group::Sensors m_sensors;
    Group::State m_state;
    Group::Name m_name;
    Group::Type m_type;
    Group::GroupClass m_groupClass;
    Group::Recycle m_recycle;
    bool m_validConstructor;

};

#endif // HUEGROUP_H
