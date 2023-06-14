#pragma once

#include <QAbstractItemModel>
#include <QSqlDatabase>

#include "DatabaseListModelItem.h"

class QSqlQueryModel;
class QSqlTableModel;

enum Roles
{
    IdRole = Qt::UserRole + 1,

    CountryIdRole,
    CountryCodeRole,
    NameRole
};

class DatabaseListModel : public QAbstractItemModel
{
public:
    DatabaseListModel(QSqlDatabase &database, QObject *parent);
    ~DatabaseListModel();

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;

    void addItem(int mcc, int mnc, const QString &name);

    QString getCode(int mcc) const;

private:
    void setupModelData();

private:
    DatabaseListModelItem *m_rootItem;

    QSqlDatabase m_database;
};
