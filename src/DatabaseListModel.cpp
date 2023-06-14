#include "DatabaseListModel.h"

#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRecord>

#include <QDebug>

DatabaseListModel::DatabaseListModel(QSqlDatabase &database, QObject *parent)
    : QAbstractItemModel(parent), m_database(database)
{
    setupModelData();
}

DatabaseListModel::~DatabaseListModel()
{
    delete m_rootItem;
}

bool DatabaseListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (role != NameRole)
        return false;

    auto item = static_cast<DatabaseListModelItem *>(index.internalPointer());
    item->setData("name", value);

    emit dataChanged(index, index, QVector<int>({NameRole}));

    QSqlQuery query(QString("UPDATE operators SET name='%1' WHERE mcc == %2 AND mnc == %3")
                        .arg(value.toString(), item->data("mcc").toString(), item->data("mnc").toString()),
                    m_database);

    auto res = query.exec();
    Q_ASSERT(res);

    return res;
}

int DatabaseListModel::rowCount(const QModelIndex &parent) const
{
    DatabaseListModelItem *parentItem;

    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<DatabaseListModelItem *>(parent.internalPointer());

    return parentItem->childCount();
}

int DatabaseListModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QHash<int, QByteArray> DatabaseListModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[IdRole]          = "mnc";
    roles[CountryIdRole]   = "mcc";
    roles[CountryCodeRole] = "code";
    roles[NameRole]        = "name";

    return roles;
}

void DatabaseListModel::addItem(int mcc, int mnc, const QString &name)
{
    for (int i = 0; i < m_rootItem->childCount(); i++)
    {
        auto country = m_rootItem->child(i);

        if (country->data("mcc").toInt() == mcc)
        {
            QMap<QString, QVariant> operatorData;
            operatorData["mcc"]  = mcc;
            operatorData["mnc"]  = mnc;
            operatorData["name"] = name;

            beginInsertRows(index(i), country->childCount(), country->childCount());
            country->appendChild(new DatabaseListModelItem(operatorData, country));
            endInsertRows();

            m_database.transaction();

            QSqlQuery query(QString("INSERT INTO operators (mcc, mnc, name) VALUES ('%1', '%2', '%3')")
                                .arg(QString::number(mcc), QString::number(mnc), name),
                            m_database);
            if (query.exec())
                m_database.commit();
            else
                qInfo() << m_database.lastError();
        }
    }
}

QString DatabaseListModel::getCode(int mcc) const
{
    for (int i = 0; i < m_rootItem->childCount(); i++)
    {
        auto country = m_rootItem->child(i);
        if (country->data("mcc").toInt() == mcc)
            return country->data("code").toString();
    }
    return {};
}

void DatabaseListModel::setupModelData()
{
    m_rootItem = new DatabaseListModelItem({});

    QSqlQuery query("SELECT * FROM countries", m_database);
    bool res = query.exec();
    Q_ASSERT(res);

    while (query.next())
    {
        QMap<QString, QVariant> data;
        int mcc      = query.value(0).toInt();
        data["mcc"]  = mcc;
        data["code"] = query.value(1).toString();
        data["name"] = query.value(2).toString();

        auto country = new DatabaseListModelItem(data, m_rootItem);

        QSqlQuery operatorsQuery("SELECT * from 'operators' WHERE mcc == " + QString::number(mcc));
        res = operatorsQuery.exec();
        Q_ASSERT(res);
        while (operatorsQuery.next())
        {
            QMap<QString, QVariant> operatorData;
            operatorData["mcc"]  = operatorsQuery.value(0).toInt();
            operatorData["mnc"]  = operatorsQuery.value(1).toString();
            operatorData["name"] = operatorsQuery.value(2).toString();

            country->appendChild(new DatabaseListModelItem(operatorData, country));
        }

        m_rootItem->appendChild(country);
    }
}

QVariant DatabaseListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto item = static_cast<DatabaseListModelItem *>(index.internalPointer());

    return item->data(roleNames().value(role));
}

Qt::ItemFlags DatabaseListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    auto item = static_cast<DatabaseListModelItem *>(index.internalPointer());

    if (item->childCount() > 0)
        return Qt::ItemIsEnabled;

    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QModelIndex DatabaseListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DatabaseListModelItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<DatabaseListModelItem *>(parent.internalPointer());

    auto childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex DatabaseListModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    auto childItem  = static_cast<DatabaseListModelItem *>(index.internalPointer());
    auto parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
