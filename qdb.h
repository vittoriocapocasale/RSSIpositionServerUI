#ifndef QDB_H
#define QDB_H
#include <QtSql>
#include <QSqlDatabase>
#include <vector>
#include <memory>
#include <sldb.h>
#include <device.h>
class QDB
{
public:
    static QDB& getInstance(){
        static SLDB sldb;
        static QDB db(sldb);
        return db;
    }
    QDB& operator=(QDB&)=delete;
    QDB (QDB &)=delete;
    //tabella autoincrement payload hash time x, y devid, fakeness
    SLDB& getDB();
    void insert(quint64 hash, quint64 time, double x, double y, qulonglong devid, int fakeness, qulonglong sequence, qulonglong iteration);
    void multipleInsert(QList<Device> & devices);
    qulonglong selectSequenceFromId(qulonglong devid);
    qulonglong selectCount(qulonglong time);
    std::tuple<qulonglong, qulonglong, qulonglong> selectCounts(qulonglong time);
    QSqlQuery& selectAll();
    qulonglong selectCountWithFakeness(qulonglong time);
    qulonglong selectCountWithoutFakeness(qulonglong time);
    std::shared_ptr<std::vector<std::tuple<qulonglong, double, double>>> selectLastPositions();
    std::shared_ptr<std::vector<std::tuple<qulonglong, double, double>>>  selectAllPositions(qulonglong devid, qulonglong from);
    std::shared_ptr<std::vector<std::tuple<qulonglong, qulonglong>>> selectMostFrequent(qulonglong from);

private:
    QDB(SLDB sldb);
    SLDB db;
    QSqlQuery selectAllQuery;
    QSqlQuery createTableQuery;
    QSqlQuery insertQuery;
    QSqlQuery selectCountWithFakenessQuery;
    QSqlQuery selectCountWithoutFakenessQuery;
    QSqlQuery selectCountQuery;
    QSqlQuery selectLastPositionsQuery;
    QSqlQuery selectAllPositionsQuery;
    QSqlQuery selectSequenceFromIdQuery;
    QSqlQuery selectMostFrequentQuery;


    const QString selectAllQueryString ="SELECT * "
                                          "FROM  packets ";
    const QString createTableQueryString = "CREATE TABLE IF NOT EXISTS packets ("
                                           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                           "hash INTEGER,"
                                           "time INTEGER,"
                                           "x REAL,"
                                           "y REAL,"
                                           "devid TEXT,"
                                           "fakeness INTEGER,"
                                           "sequence INTEGER,"
                                           "iteration INTEGER)";
    const QString insertQueryString= "INSERT INTO packets (id, hash, time, x, y, devid, fakeness, sequence, iteration) "
                                     "VALUES (null, ?,?,?,?,?,?,?,?)";
    const QString selectCountWithFakenessQueryString="SELECT COUNT(distinct packets.devid) as total "
                                                     "FROM (SELECT max(time) as time FROM packets) as temp, packets "
                                                     "WHERE packets.time=temp.time AND packets.time=? AND packets.fakeness=2";
    const QString selectCountWithoutFakenessQueryString = "SELECT COUNT(distinct packets.devid) as total "
                                                          "FROM (SELECT max(time) as time FROM packets) as temp, packets "
                                                          "WHERE packets.time=temp.time AND packets.time=? AND packets.fakeness=0";
    const QString selectCountQueryString ="SELECT COUNT(distinct packets.devid) as total "
                                          "FROM (SELECT max(time) as time FROM packets) as temp, packets "
                                          "WHERE packets.time=temp.time AND packets.time=?";



    const QString selectSequenceFromIdQueryString = "SELECT max(packets.sequence) as sequence "
                                                   "FROM packets, (SELECT max (time) as time "
                                                                  "FROM  packets) as temp "
                                                   "WHERE packets.devid=? AND packets.time=temp.time ";
    const QString selectLastPositionsQueryString = "SELECT packets.devid, packets.x, packets.y "
                                                   "FROM packets, (SELECT max (packets.id) as id "
                                                                  "FROM  (SELECT  max(time) as time "
                                                                         "FROM packets) as temp2, packets "
                                                                  "WHERE packets.time=temp2.time "
                                                                  "GROUP BY devid) as temp "
                                                   "WHERE packets.id=temp.id ";
    const QString selectAllPositionsQueryString= "SELECT packets.time, packets.x, packets.y "
                                                 "FROM packets "
                                                 "WHERE packets.devid=? AND packets.time>? "
                                                 "ORDER BY packets.time ASC ";

    const QString selectMostFrequentQueryString ="SELECT packets.devid as id, packets.time as time "
                                                 "FROM packets,(SELECT devid, count (distinct time/86400) as total "
                                                                "FROM packets "
                                                                "WHERE packets.time>? "
                                                                "GROUP BY devid "
                                                                "ORDER BY total DESC "
                                                                "LIMIT 5) as temp, "
                                                               "(SELECT max(id) as id "
                                                                "FROM packets "
                                                                "WHERE packets.time>? "
                                                                "GROUP BY devid, time/86400 ) as temp2 "
                                                 "WHERE packets.devid=temp.devid and packets.id=temp2.id "
                                                 "ORDER BY packets.devid, packets.time ASC";
};

#endif // QDB_H
