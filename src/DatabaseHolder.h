#pragma once

#include <QObject>
#include <QSqlDatabase>

class DatabaseHolder : public QObject
{
    Q_OBJECT
public:
    ~DatabaseHolder();
    static DatabaseHolder& getInstance();

    QSqlDatabase getDatabase() const;

    bool init();

private:
    DatabaseHolder();
    Q_DISABLE_COPY_MOVE(DatabaseHolder);

    QSqlDatabase m_database;
};
