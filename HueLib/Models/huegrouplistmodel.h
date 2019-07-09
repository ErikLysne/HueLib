#ifndef HUEGROUPLISTMODEL_H
#define HUEGROUPLISTMODEL_H

#include <QAbstractListModel>

#include "../hueobjectlist.h"

class HueGroupListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit HueGroupListModel(std::shared_ptr<HueGroupList> group, QObject* parent = nullptr);
    ~HueGroupListModel() override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    std::shared_ptr<HueGroup> getGroup(const QModelIndex& index) const;
private:
    std::shared_ptr<HueGroupList> m_groupList;

};

#endif // HUEGROUPLISTMODEL_H
