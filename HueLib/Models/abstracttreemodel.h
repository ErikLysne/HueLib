#ifndef ABSTRACTTREEMODEL_H
#define ABSTRACTTREEMODEL_H

#include <QAbstractItemModel>
#include <QHash>
#include <memory>

class TreeItem;

class AbstractTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit AbstractTreeModel(QObject* parent = nullptr);
    virtual ~AbstractTreeModel() override;

    QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

protected:
    void setRootItem(TreeItem* rootItem);
    TreeItem* getRootItem();


private:
    virtual void setupModelData(TreeItem* rootItem) = 0;

private:
    TreeItem* m_rootItem;
};

#endif // ABSTRACTTREEMODEL_H
