#ifndef HUELIGHTMODEL_H
#define HUELIGHTMODEL_H

#include "abstracttreemodel.h"

class HueLight;

class HueLightInfoTreeModel : public AbstractTreeModel
{
    Q_OBJECT
public:
    explicit HueLightInfoTreeModel(std::shared_ptr<HueLight> light, QObject* parent = nullptr);
    ~HueLightInfoTreeModel() override;

private:
    void setupModelData(TreeItem* rootItem) override;

private:
    std::shared_ptr<HueLight> m_light;
};

#endif // HUELIGHTMODEL_H
