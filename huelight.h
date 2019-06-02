#ifndef HUELIGHT_H
#define HUELIGHT_H

#include "hueabstractobject.h"
#include "huetypes.h"

class HueLight : public HueAbstractObject
{
    Q_OBJECT
public:
    static QList<HueLight*> discoverLights(HueBridge* bridge);

    bool hasValidConstructor() const;
    int ID() const;
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

    bool synchronize() override;

protected:
    static bool constructHueLight(int ID, QJsonObject json, HueLight* light);

    HueLight(HueBridge* bridge);
    HueLight(HueBridge* bridge,
             int ID,
             Light::State state, Light::Name name,
             Light::Type type, Light::UniqueID uniqueID,
             Light::SoftwareVersion softwareVersion,
             Light::SoftwareUpdate softwareUpdate,
             Light::SoftwareConfigID softwareConfigID,
             Light::ProductName productName,
             Light::Manufacturer manufacturer,
             Light::ProductID productID);
    HueLight(const HueLight& rhs);
    HueLight operator=(const HueLight& rhs);

    HueRequest makePutRequest(QJsonObject json) override;
    HueRequest makeGetRequest() override;

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
    bool m_validConstructor;

};

#endif // HUELIGHT_H
