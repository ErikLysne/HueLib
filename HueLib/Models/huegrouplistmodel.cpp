#include "huegrouplistmodel.h"

#include "../huegroup.h"

HueGroupListModel::HueGroupListModel(std::shared_ptr<HueGroupList> group, QObject* parent)
    : QAbstractListModel(parent)
    , m_groupList(group)
{

}

HueGroupListModel::~HueGroupListModel()
{
    m_groupList.reset();
}

int HueGroupListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_groupList->size();
}

QVariant HueGroupListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return m_groupList->at(index.row())->name().getName();
    default:
        return QVariant();
    }
}

QVariant HueGroupListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return "Groups";

    return QVariant();
}

std::shared_ptr<HueGroup> HueGroupListModel::getGroup(const QModelIndex &index) const
{
    return m_groupList->at(index.row());
}
