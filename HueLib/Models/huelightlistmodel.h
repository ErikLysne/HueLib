#ifndef HUELIGHTLISTMODEL_H
#define HUELIGHTLISTMODEL_H

#include <QAbstractListModel>

#include "../hueobjectlist.h"

class HueLightListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit HueLightListModel(std::shared_ptr<HueLightList> lightList, QObject* parent = nullptr);
    ~HueLightListModel() override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    std::shared_ptr<HueLight> getLight(const QModelIndex& index) const;

private:
    std::shared_ptr<HueLightList> m_lightList;
};

#endif // HUELIGHTLISTMODEL_H
