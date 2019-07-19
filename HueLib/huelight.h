#ifndef HUELIGHT_H
#define HUELIGHT_H

#include "hueabstractobject.h"
#include "huetypes.h"
#include "hueobjectlist.h"

class HueLight : public HueAbstractObject
{
    Q_OBJECT
public:
    HueLight();
    HueLight(HueBridge* bridge);

    static HueLightList discoverLights(HueBridge* bridge);

    Light::State state() const;
    Light::Name name() const;
    Light::Type type() const;
    Light::UniqueID uniqueID() const;
    Light::SoftwareVersion softwareVersion() const;
    Light::SoftwareUpdate softwareUpdate() const;
    Light::SoftwareConfigID softwareConfigID() const;
    Light::ProductName productName() const;
    Light::Manufacturer manufacturer() const;
    Light::ProductID productID() const;
    Light::Config config() const;

    bool hasValidConstructor() const override;
    bool isValid() const override;
    int ID() const override;
    bool synchronize() override;

private:
    HueLight(HueBridge* bridge,
             int ID,
             Light::State state, Light::Name name,
             Light::Type type, Light::UniqueID uniqueID,
             Light::SoftwareVersion softwareVersion,
             Light::SoftwareUpdate softwareUpdate,
             Light::SoftwareConfigID softwareConfigID,
             Light::ProductName productName,
             Light::Manufacturer manufacturer,
             Light::ProductID productID,
             Light::Config config);
    HueLight(const HueLight& rhs);
    HueLight operator=(const HueLight& rhs);

    static bool constructHueLight(int ID, QJsonObject json, std::shared_ptr<HueLight>& light);

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
    Light::State m_state;
    Light::Name m_name;
    Light::Type m_type;
    Light::UniqueID m_uniqueID;
    Light::SoftwareVersion m_softwareVersion;
    Light::SoftwareUpdate m_softwareUpdate;
    Light::SoftwareConfigID m_softwareConfigID;
    Light::ProductName m_productName;
    Light::Manufacturer m_manufacturer;
    Light::ProductID m_productID;
    Light::Config m_config;
    bool m_validConstructor;

};

#endif // HUELIGHT_H
