#ifndef HUEGROUP_H
#define HUEGROUP_H

#include <memory>

#include "hueabstractobject.h"
#include "huetypes.h"
#include "hueobjectlist.h"

class HueGroup : public HueAbstractObject
{
    Q_OBJECT
public:
    HueGroup();
    HueGroup(HueBridge* bridge);

    static HueGroupList discoverGroups(HueBridge* bridge);

    void linkToLights(const HueLightList& lights);
    HueLightList getLights(const HueLightList& lights) const;

    Group::Action action() const;
    Group::Lights lights() const;
    Group::Sensors sensors() const;
    Group::State state() const;
    Group::Name name() const;
    Group::Type type() const;
    Group::GroupClass groupClass() const;
    Group::Recycle recycle() const;

    bool hasValidConstructor() const override;
    bool isValid() const override;
    int ID() const override;
    bool synchronize() override;

private:
    HueGroup(HueBridge* bridge,
             int ID,
             Group::Action action, Group::Lights lights,
             Group::Sensors sensors, Group::State state,
             Group::Name name, Group::Type type,
             Group::GroupClass classObject, Group::Recycle recycle);
    HueGroup(const HueGroup& rhs);
    HueGroup operator=(const HueGroup& rhs);

    static bool constructHueGroup(int ID, QJsonObject json, std::shared_ptr<HueGroup>& group);

    HueRequest makePutRequest(QJsonObject json) override;
    HueRequest makeGetRequest() override;

    void updateOn(const bool on) override;
    void updateHue(const int hue) override;
    void updateSaturation(const int saturation) override;
    void updateBrightness(const int brightness) override;
    void updateColorTemp(const int colorTemp) override;
    void updateXY(const double x, const double y) override;
    void updateAlert(const HueAlert alert) override;
    void updateEffect(const HueEffect effect) override;

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
