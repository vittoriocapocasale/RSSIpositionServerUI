#ifndef SLDB_H
#define SLDB_H
#include <QtSql>
#include <QSqlDatabase>

class SLDB
{
public:
    SLDB();
    QSqlDatabase& get();
private:
    QSqlDatabase db;
};

#endif // SLDB_H
