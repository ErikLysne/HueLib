#ifndef HUELIGHTMODEL_H
#define HUELIGHTMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <memory>

#include "huelight.h"

class HueLightModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        OnRole = Qt::UserRole + 1,
        ReachableRole,
        BrightnessRole,
        HueRole,
        SaturationRole,
        ColorTempRole,
        XRole,
        YRole,
        EffectRole,
        AlertRole,
        ColorModeRole,
        ModeRole
    };

    HueLightModel(std::shared_ptr<HueLight> light, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    bool removeRows(int row, int count, const QModelIndex& parent) override;
    QHash<int, QByteArray> roleNames() const override;

private:
    bool isIndexValid(const QModelIndex& index) const;

private:
    std::shared_ptr<HueLight> m_light;
};

#endif // HUELIGHTMODEL_H
