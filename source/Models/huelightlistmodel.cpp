#include "huelightlistmodel.h"

#include "../huelight.h"

HueLightListModel::HueLightListModel(std::shared_ptr<HueLightList> lightList, QObject* parent)
    : QAbstractListModel(parent)
    , m_lightList(lightList)
{

}

HueLightListModel::~HueLightListModel()
{
    m_lightList.reset();
}

int HueLightListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_lightList->size();
}

QVariant HueLightListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    {
        auto light = m_lightList->at(index.row());
        return QString(QString::number(light->ID()) + ": " + light->name().getName());
    }
    default:
        return QVariant();
    }
}

QVariant HueLightListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return "Lights";

    return QVariant();
}

std::shared_ptr<HueLight> HueLightListModel::getLight(const QModelIndex &index) const
{
    return m_lightList->at(index.row());
}
