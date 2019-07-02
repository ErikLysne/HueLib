#ifndef HUEGROUPMODEL_H
#define HUEGROUPMODEL_H


#include "abstracttreemodel.h"

class HueGroup;

class HueGroupModel : public AbstractTreeModel
{
    Q_OBJECT
public:
    explicit HueGroupModel(std::shared_ptr<HueGroup> group, QObject* parent = nullptr);
    ~HueGroupModel() override;

private:
    void setupModelData(TreeItem* rootItem) override;

private:
    std::shared_ptr<HueGroup> m_group;
};

#endif // HUEGROUPMODEL_H
