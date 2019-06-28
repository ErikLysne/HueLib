#include "huelightmodel.h"

HueLightModel::HueLightModel(std::shared_ptr<HueLight> light, QObject* parent) :
    QAbstractListModel (parent),
    m_light(light)
{

}

int HueLightModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 12;
}

QVariant HueLightModel::data(const QModelIndex& index, int role) const
{
    if (!isIndexValid(index)) {
        return QVariant();
    }

    switch (role) {
    case Roles::OnRole:
        return m_light->state().isOn();
    case Roles::ReachableRole:
        return m_light->state().isReachable();
    case Roles::BrightnessRole:
        return m_light->state().getBrightness();
    case Roles::HueRole:
        return m_light->state().getHue();
    case Roles::SaturationRole:
        return m_light->state().getSaturation();
    case Roles::ColorTempRole:
        return m_light->state().getColorTemp();
    case Roles::XRole:
        return m_light->state().getXValue();
    case Roles::YRole:
        return m_light->state().getYValue();
    case Roles::EffectRole:
        return m_light->state().getEffect();
    case Roles::ColorModeRole:
        return m_light->state().getColorMode();
    case Roles::ModeRole:
        return m_light->state().getMode();
    default:
        return QVariant();
    }
}

bool HueLightModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

}

bool HueLightModel::removeRows(int row, int count, const QModelIndex &parent)
{

}

QHash<int, QByteArray> HueLightModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::OnRole] = "on";
    roles[Roles::ReachableRole] = "reachable";
    roles[Roles::BrightnessRole] = "brightness";
    roles[Roles::HueRole] = "hue";
    roles[Roles::SaturationRole] = "saturation";
    roles[Roles::ColorTempRole] = "colortemp";
    roles[Roles::XRole] = "xvalue";
    roles[Roles::YRole] = "yrole";
    roles[Roles::EffectRole] = "effect";
    roles[Roles::AlertRole] = "alert";
    roles[Roles::ColorModeRole] = "colormode";
    roles[Roles::ModeRole] = "mode";

    return roles;
}

bool HueLightModel::isIndexValid(const QModelIndex &index) const
{
    return true;
}
