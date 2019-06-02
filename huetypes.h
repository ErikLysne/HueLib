#ifndef HUETYPES_H
#define HUETYPES_H

#include <QString>
#include <QJsonObject>
#include <QList>

/* =====================================
 * =============== LIGHT ===============
 * =====================================
 *
 */

namespace Light {

class State
{
public:
    State();
    State(const QJsonValue json);

    bool isOn() const;
    bool isReachable() const;
    int getBrightness() const;
    int getHue() const ;
    int getSaturation() const;
    int getColorTemp() const;
    double getXValue() const;
    double getYValue() const;
    QString getEffect() const;
    QString getAlert() const;
    QString getColorMode() const;
    QString getMode() const;
    void setOn(const bool on);
    void setReachable(const bool reachable);
    void setBrightness(const int brightness);
    void setHue(const int hue);
    void setSaturation(const int saturation);
    void setColorTemp(const int colorTemp);
    void setXValue(const double xValue);
    void setYValue(const double yValue);
    void setEffect(const QString effect);
    void setAlert(const QString alert);
    void setColorMode(const QString colorMode);
    void setMode(const QString mode);

private:
    bool m_on;
    bool m_reachable;
    int m_brightness;
    int m_hue;
    int m_saturation;
    int m_colorTemp;
    double m_xValue;
    double m_yValue;
    QString m_effect;
    QString m_alert;
    QString m_colorMode;
    QString m_mode;
};

class Name {
public:
    Name();
    Name(const QJsonValue json);

    QString getName() const;
    void setName(const QString name);

private:
    QString m_name;
};

class Type {
public:
    Type();
    Type(const QJsonValue json);

    QString getType() const;
    void setType(const QString type);

private:
    QString m_type;
};

class UniqueID {
public:
    UniqueID();
    UniqueID(const QJsonValue json);

    QString getUniqueID() const;
    void setUniqueID(const QString uniqueID);

private:
    QString m_uniqueID;
};

class SoftwareVersion
{
public:
    SoftwareVersion();
    SoftwareVersion(const QJsonValue json);

    QString getSoftwareVersion() const;
    void setSoftwareVersion(const QString softwareVersion);

private:
    QString m_softwareVersion;
};

class SoftwareUpdate
{
public:
    SoftwareUpdate();
    SoftwareUpdate(const QJsonValue json);

    QString getState() const;
    QString getLastInstall() const;
    void setState(const QString state);
    void setLastInstall(const QString lastInstall);

private:
    QString m_state;
    QString m_lastInstall;

};

class SoftwareConfigID
{
public:
    SoftwareConfigID();
    SoftwareConfigID(const QJsonValue json);

    QString getSoftwareConfigID() const;
    void setSoftwareConfigID(const QString softwareConfigID);

private:
    QString m_softwareConfigID;
};

class ProductName
{
public:
    ProductName();
    ProductName(const QJsonValue json);

    QString getProductName() const;
    void setProductName(const QString productName);

private:
    QString m_productName;
};

class Manufacturer
{
public:
    Manufacturer();
    Manufacturer(const QJsonValue json);

    QString getManufacturer() const;
    void setManufacturer(const QString manufacturer);

private:
    QString m_manufacturer;
};

class ProductID
{
public:
    ProductID();
    ProductID(const QJsonValue json);

    QString getProductID() const;
    void setProductID(const QString productID);

private:
    QString m_productID;
};

}

/* =====================================
 * =============== GROUP ===============
 * =====================================
 *
 */

namespace Group {

class Action
{
public:
    Action();
    Action(const QJsonValue json);

    bool isOn() const;
    int getBrightness() const;
    int getHue() const ;
    int getSaturation() const;
    int getColorTemp() const;
    double getXValue() const;
    double getYValue() const;
    QString getEffect() const;
    QString getAlert() const;
    QString getColorMode() const;
    void setOn(const bool on);
    void setBrightness(const int brightness);
    void setHue(const int hue);
    void setSaturation(const int saturation);
    void setColorTemp(const int colorTemp);
    void setXValue(const double xValue);
    void setYValue(const double yValue);
    void setEffect(const QString effect);
    void setColorMode(const QString colorMode);
    void setAlert(const QString alert);

private:
    bool m_on;
    int m_brightness;
    int m_hue;
    int m_saturation;
    int m_colorTemp;
    double m_xValue;
    double m_yValue;
    QString m_effect;
    QString m_alert;
    QString m_colorMode;
};

class Lights
{
public:
    Lights();
    Lights(const QJsonValue json);

    QList<QString> getLights() const;
    void setLights(const QList<QString> lights);

private:
    QList<QString> m_lights;
};

class Sensors
{
public:
    Sensors();
    Sensors(const QJsonValue json);

    QList<QString> getSensors() const;
    void setSensors(const QList<QString> sensors);

private:
    QList<QString> m_sensors;
};

class State
{
public:
    State();
    State(const QJsonValue json);

    bool getAllOn() const;
    bool getAnyOn() const;
    void setAllOn(const bool allOn);
    void setAnyOn(const bool anyOn);

private:
    bool m_allOn;
    bool m_anyOn;
};

class Name
{
public:
    Name();
    Name(const QJsonValue json);

    QString getName() const;
    void setName(const QString name);

private:
    QString m_name;
};

class Type
{
public:
    Type();
    Type(const QJsonValue json);

    QString getType() const;
    void setType(const QString type);

private:
    QString m_type;
};

class GroupClass
{
public:
    GroupClass();
    GroupClass(const QJsonValue json);

    QString getGroupClass() const;
    void setGroupClass(const QString groupClass);

private:
    QString m_groupClass;
};

class Recycle
{
public:
    Recycle();
    Recycle(const QJsonValue json);

    bool getRecycle() const;
    void setRecycle(const bool recycle);

private:
    bool m_recycle;
};

}



#endif // HUETYPES_H
