#include "huelightmodel.h"

HueLightModel::HueLightModel(std::shared_ptr<HueLight> light, QObject* parent) :
    AbstractTreeModel(parent),
    m_light(light)
{
    setRootItem(new TreeItem({tr("Parameter"), tr("Value")}));
    setupModelData(getRootItem());
}

HueLightModel::~HueLightModel()
{
    m_light.reset();
}

void HueLightModel::setupModelData(TreeItem* rootItem)
{
    Light::Name name = m_light->name();
    Light::Type type = m_light->type();
    Light::UniqueID uniqueID = m_light->uniqueID();
    TreeItem* aboutItem = new TreeItem({tr("Info"), tr("")}, rootItem);
    aboutItem->appendChild(new TreeItem({tr("Name"),        name.getName()},            aboutItem));
    aboutItem->appendChild(new TreeItem({tr("Type"),        type.getType()},            aboutItem));
    aboutItem->appendChild(new TreeItem({tr("Unique ID"),   uniqueID.getUniqueID()},    aboutItem));

    Light::State state = m_light->state();
    TreeItem* stateItem = new TreeItem({tr("State"), tr("")}, rootItem);
    stateItem->appendChild(new TreeItem({tr("On"),          state.isOn()},          stateItem));
    stateItem->appendChild(new TreeItem({tr("Reachable"),   state.isReachable()},   stateItem));
    stateItem->appendChild(new TreeItem({tr("Brightness"),  state.getBrightness()}, stateItem));
    stateItem->appendChild(new TreeItem({tr("Hue"),         state.getHue()},        stateItem));
    stateItem->appendChild(new TreeItem({tr("Saturation"),  state.getHue()},        stateItem));
    stateItem->appendChild(new TreeItem({tr("Color temp"),  state.getColorTemp()},  stateItem));
    stateItem->appendChild(new TreeItem({tr("X"),           state.getXValue()},     stateItem));
    stateItem->appendChild(new TreeItem({tr("Y"),           state.getYValue()},     stateItem));
    stateItem->appendChild(new TreeItem({tr("Effect"),      state.getEffect()},     stateItem));
    stateItem->appendChild(new TreeItem({tr("Alert"),       state.getAlert()},      stateItem));
    stateItem->appendChild(new TreeItem({tr("Colormode"),   state.getColorMode()},  stateItem));
    stateItem->appendChild(new TreeItem({tr("Mode"),        state.getMode()},       stateItem));

    Light::SoftwareVersion swVersion = m_light->softwareVersion();
    Light::SoftwareUpdate swUpdate = m_light->softwareUpdate();
    Light::SoftwareConfigID swConfigID = m_light->softwareConfigID();
    TreeItem* swItem = new TreeItem({tr("Software"), tr("")}, rootItem);
    swItem->appendChild(new TreeItem({tr("Version"),            swVersion.getSoftwareVersion()},    swItem));
    swItem->appendChild(new TreeItem({tr("Last update"),        swUpdate.getLastInstall()},         swItem));
    swItem->appendChild(new TreeItem({tr("Pending updates"),    swUpdate.getState()},               swItem));
    swItem->appendChild(new TreeItem({tr("Config ID"),          swConfigID.getSoftwareConfigID()},  swItem));

    Light::ProductName productName = m_light->productName();
    Light::Manufacturer manufacturer = m_light->manufacturer();
    Light::ProductID productID = m_light->productID();
    TreeItem* modelItem = new TreeItem({tr("Light model"), tr("")}, rootItem);
    modelItem->appendChild(new TreeItem({tr("Product name"),    productName.getProductName()},      modelItem));
    modelItem->appendChild(new TreeItem({tr("Manufacturer"),    manufacturer.getManufacturer()},    modelItem));
    modelItem->appendChild(new TreeItem({tr("Product ID"),      productID.getProductID()},          modelItem));

    rootItem->appendChild(aboutItem);
    rootItem->appendChild(stateItem);
    rootItem->appendChild(swItem);
    rootItem->appendChild(modelItem);
}
