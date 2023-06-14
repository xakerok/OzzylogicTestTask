#pragma once

#include <QVariant>
#include <QList>

class DatabaseListModelItem
{
public:
    explicit DatabaseListModelItem(const QMap<QString, QVariant> &data, DatabaseListModelItem *parentItem = nullptr);
    ~DatabaseListModelItem();

    void appendChild(DatabaseListModelItem *child);

    DatabaseListModelItem *child(int row);
    int childCount() const;

    int dataCount() const;
    QVariant data(QString key) const;
    void setData(const QString &key, const QVariant &value);

    int row() const;

    DatabaseListModelItem *parentItem();

private:
    QList<DatabaseListModelItem *> m_childItems;
    QMap<QString, QVariant> m_itemData;
    DatabaseListModelItem *m_parentItem;
};
