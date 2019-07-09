#ifndef HUEGROUPMODEL_H
#define HUEGROUPMODEL_H

#include "abstracttreemodel.h"

class HueGroup;

class HueGroupInfoTreeModel : public AbstractTreeModel
{
    Q_OBJECT
public:
    explicit HueGroupInfoTreeModel(std::shared_ptr<HueGroup> group, QObject* parent = nullptr);
    ~HueGroupInfoTreeModel() override;

private slots:
    void reset();

private:
    void setupModelData(TreeItem* rootItem) override;

private:
    std::shared_ptr<HueGroup> m_group;
};

#endif // HUEGROUPMODEL_H
