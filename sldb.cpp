#include "sldb.h"

SLDB::SLDB()
{
    db= QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./sqlDb3");
    if (!db.open())
    {
        throw std::runtime_error("DB open failed");
    }
}

QSqlDatabase& SLDB::get()
{
    return this->db;
}
