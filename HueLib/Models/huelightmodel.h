#ifndef HUELIGHTMODEL_H
#define HUELIGHTMODEL_H

#include "abstracttreemodel.h"

class HueLight;

class HueLightModel : public AbstractTreeModel
{
    Q_OBJECT
public:
    explicit HueLightModel(std::shared_ptr<HueLight> light, QObject* parent = nullptr);
    ~HueLightModel() override;

private:
    void setupModelData(TreeItem* rootItem) override;

private:
    std::shared_ptr<HueLight> m_light;
};

#endif // HUELIGHTMODEL_H
