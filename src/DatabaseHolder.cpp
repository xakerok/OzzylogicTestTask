#include "DatabaseHolder.h"

#include <QApplication>
#include <QDebug>
#include <QSqlError>
#include <QFile>

DatabaseHolder::~DatabaseHolder()
{
    m_database.close();
}

DatabaseHolder &DatabaseHolder::getInstance()
{
    static QScopedPointer<DatabaseHolder> holder(new DatabaseHolder);
    return *holder;
}

QSqlDatabase DatabaseHolder::getDatabase() const
{
    return m_database;
}

DatabaseHolder::DatabaseHolder()
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
}

bool DatabaseHolder::init()
{
    QString path(qApp->applicationDirPath() + "/system.db");

    if (!QFile::exists(path))
    {
        qInfo() << "Database file is not found";
        return false;
    }

    m_database.setDatabaseName(path);

    if (!m_database.open())
    {
        qInfo() << "Database is not open: " << m_database.lastError().text();
        return false;
    }

    return true;
}
