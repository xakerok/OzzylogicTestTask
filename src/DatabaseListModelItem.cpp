#include "DatabaseListModelItem.h"

DatabaseListModelItem::DatabaseListModelItem(const QMap<QString, QVariant> &data, DatabaseListModelItem *parentItem)
    : m_itemData(data), m_parentItem(parentItem)
{
}

DatabaseListModelItem::~DatabaseListModelItem()
{
    qDeleteAll(m_childItems);
}

void DatabaseListModelItem::appendChild(DatabaseListModelItem *child)
{
    m_childItems.append(child);
}

DatabaseListModelItem *DatabaseListModelItem::child(int row)
{
    Q_ASSERT(row >= 0 && row < m_childItems.size());

    return m_childItems.at(row);
}

int DatabaseListModelItem::childCount() const
{
    return m_childItems.size();
}

int DatabaseListModelItem::dataCount() const
{
    return m_itemData.size();
}

QVariant DatabaseListModelItem::data(QString key) const
{
    return m_itemData.value(key);
}

void DatabaseListModelItem::setData(const QString &key, const QVariant &value)
{
    m_itemData[key] = value;
}

int DatabaseListModelItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<DatabaseListModelItem *>(this));

    return 0;
}

DatabaseListModelItem *DatabaseListModelItem::parentItem()
{
    return m_parentItem;
}
