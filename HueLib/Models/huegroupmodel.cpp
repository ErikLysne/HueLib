#include "huegroupmodel.h"

#include "treeitem.h"
#include "../huegroup.h"

HueGroupModel::HueGroupModel(std::shared_ptr<HueGroup> group, QObject* parent) :
    AbstractTreeModel(parent),
    m_group(group)
{
    setRootItem(new TreeItem({tr("Parameter"), tr("Value")}));
    setupModelData(getRootItem());
}

HueGroupModel::~HueGroupModel()
{
    m_group.reset();
}

void HueGroupModel::setupModelData(TreeItem *rootItem)
{
    int ID = m_group->ID();
    Group::Name name = m_group->name();
    Group::Type type = m_group->type();
    Group::GroupClass groupClass = m_group->groupClass();
    Group::Recycle recycle = m_group->recycle();
    TreeItem* aboutItem = new TreeItem({tr("About"), tr("")}, rootItem);
    aboutItem->appendChild(new TreeItem({tr("ID"),      ID},                            aboutItem));
    aboutItem->appendChild(new TreeItem({tr("Name"),    name.getName()},                aboutItem));
    aboutItem->appendChild(new TreeItem({tr("Type"),    type.getType()},                aboutItem));
    aboutItem->appendChild(new TreeItem({tr("Class"),   groupClass.getGroupClass()},    aboutItem));
    aboutItem->appendChild(new TreeItem({tr("Recycle"), recycle.getRecycle()},          aboutItem));

    Group::Action action = m_group->action();
    TreeItem* actionItem = new TreeItem({tr("Action"), tr("")}, rootItem);
    actionItem->appendChild(new TreeItem({tr("On"),         action.isOn()},             actionItem));
    actionItem->appendChild(new TreeItem({tr("Brightness"), action.getBrightness()},    actionItem));
    actionItem->appendChild(new TreeItem({tr("Hue"),        action.getHue()},           actionItem));
    actionItem->appendChild(new TreeItem({tr("Saturation"), action.getSaturation()},    actionItem));
    actionItem->appendChild(new TreeItem({tr("Color temp"), action.getColorTemp()},     actionItem));
    actionItem->appendChild(new TreeItem({tr("X"),          action.getXValue()},        actionItem));
    actionItem->appendChild(new TreeItem({tr("Y"),          action.getYValue()},        actionItem));
    actionItem->appendChild(new TreeItem({tr("Effect"),     action.getEffect()},        actionItem));
    actionItem->appendChild(new TreeItem({tr("Alert"),      action.getAlert()},         actionItem));
    actionItem->appendChild(new TreeItem({tr("Colormode"),  action.getColorMode()},     actionItem));

    Group::State state = m_group->state();
    TreeItem* stateItem = new TreeItem({tr("State"), tr("")}, rootItem);
    stateItem->appendChild(new TreeItem({tr("All on"),  state.getAllOn()},  stateItem));
    stateItem->appendChild(new TreeItem({tr("Any on"),  state.getAnyOn()},  stateItem));

    Group::Lights lights = m_group->lights();
    TreeItem* lightsItem = new TreeItem({tr("Lights"), tr("")}, rootItem);

    for(auto light : lights.getLights()) {
        lightsItem->appendChild(new TreeItem({light, tr("")}, lightsItem));
    }

    Group::Sensors sensors = m_group->sensors();
    TreeItem* sensorsItem = new TreeItem({tr("Sensors"), tr("")}, rootItem);
    for(auto sensor : sensors.getSensors()) {
        sensorsItem->appendChild(new TreeItem({sensor, tr("")}, sensorsItem));
    }

    rootItem->appendChild(aboutItem);
    rootItem->appendChild(actionItem);
    rootItem->appendChild(stateItem);
    rootItem->appendChild(lightsItem);
    rootItem->appendChild(sensorsItem);
}
