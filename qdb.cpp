#include "qdb.h"
#define LOG_FAILED_QUERY(query) qDebug() << __FILE__ << __LINE__ << "FAILED QUERY [" \
        << (query).executedQuery() << "]" << (query).lastError()


QDB::QDB(SLDB sldb):db(sldb), selectAllQuery(sldb.get()),createTableQuery(sldb.get()), insertQuery(sldb.get()),
    selectCountWithFakenessQuery(sldb.get()),selectCountWithoutFakenessQuery(sldb.get()),
    selectCountQuery(sldb.get()), selectLastPositionsQuery(sldb.get()),
    selectAllPositionsQuery(sldb.get()), selectSequenceFromIdQuery(sldb.get()),
    selectMostFrequentQuery(sldb.get())
{

    this->selectAllQuery.setForwardOnly(true);
    this->selectAllQuery.prepare(this->selectAllQueryString);

    this->insertQuery.setForwardOnly(true);
    this->insertQuery.prepare(this->insertQueryString);

    this->selectCountQuery.setForwardOnly(true);
    this->selectCountQuery.prepare(this->selectCountQueryString);

    this->selectCountWithFakenessQuery.setForwardOnly(true);
    this->selectCountWithFakenessQuery.prepare(this->selectCountWithFakenessQueryString);

    this->selectCountWithoutFakenessQuery.setForwardOnly(true);
    this->selectCountWithoutFakenessQuery.prepare(this->selectCountWithoutFakenessQueryString);

    this->selectLastPositionsQuery.setForwardOnly(true);
    this->selectLastPositionsQuery.prepare(this->selectLastPositionsQueryString);

    this->selectAllPositionsQuery.setForwardOnly(true);
    this->selectAllPositionsQuery.prepare(this->selectAllPositionsQueryString);

    this->selectSequenceFromIdQuery.setForwardOnly(true);
    this->selectSequenceFromIdQuery.prepare(this->selectSequenceFromIdQueryString);

    this->selectMostFrequentQuery.setForwardOnly(true);
    this->selectMostFrequentQuery.prepare(this->selectMostFrequentQueryString);

    this->createTableQuery.setForwardOnly(true);
    if (!this->createTableQuery.exec(this->createTableQueryString))
    {
        LOG_FAILED_QUERY(createTableQuery);
        throw std::runtime_error("table create failed");
    }

}

SLDB& QDB::getDB()
{
    return this->db;
}

QSqlQuery& QDB::selectAll(){
    if(!this->selectAllQuery.exec())
    {
       LOG_FAILED_QUERY(selectAllQuery);
    }
    return selectAllQuery;
}

void QDB::insert(quint64 hash, quint64 time, double x, double y, qulonglong devid, int fakeness, qulonglong sequence, qulonglong  iteration)
{
    try {
        this->insertQuery.addBindValue(hash);
        this->insertQuery.addBindValue(time);
        this->insertQuery.addBindValue(x);
        this->insertQuery.addBindValue(y);
        this->insertQuery.addBindValue(devid);
        this->insertQuery.addBindValue(fakeness);
        this->insertQuery.addBindValue(sequence);
        this->insertQuery.addBindValue(iteration);
        if (!this->insertQuery.exec())
        {
            LOG_FAILED_QUERY(insertQuery);
            //throw std::runtime_error("table insert failed");
        }
    }
    catch (...) {
        //qDebug()<<"InsertFail";
        LOG_FAILED_QUERY(insertQuery);
    }

}
qulonglong QDB::selectCount(qulonglong time)
{
    this->selectCountQuery.addBindValue(time);
    if (!this->selectCountQuery.exec())
    {
        LOG_FAILED_QUERY(selectCountQuery);
        return 0;
    }
    if (selectCountQuery.next()) {
        return  this->selectCountQuery.value(0).toULongLong();
    }
    return 0;

}
qulonglong QDB::selectCountWithFakeness(qulonglong time)
{
    this->selectCountWithFakenessQuery.addBindValue(time);
    if (!this->selectCountWithFakenessQuery.exec())
    {
        LOG_FAILED_QUERY(selectCountWithFakenessQuery);
        return 0;
    }
    if(selectCountWithFakenessQuery.next())
    {
        return  this->selectCountWithFakenessQuery.value(0).toULongLong();
    }
    return 0;
}
qulonglong QDB::selectCountWithoutFakeness(qulonglong time)
{
    this->selectCountWithoutFakenessQuery.addBindValue(time);
    if (!this->selectCountWithoutFakenessQuery.exec())
    {
        LOG_FAILED_QUERY(selectCountWithoutFakenessQuery);
        return 0;
    }
    if(selectCountWithoutFakenessQuery.next())
    {
        return  this->selectCountWithoutFakenessQuery.value(0).toULongLong();
    }
    return 0;
}
std::shared_ptr<std::vector<std::tuple<qulonglong, double, double>>> QDB::selectLastPositions()
{
    auto ret=std::make_shared<std::vector<std::tuple<qulonglong, double, double>>>();
    if (!this->selectLastPositionsQuery.exec())
    {
        LOG_FAILED_QUERY(selectLastPositionsQuery);
        return ret;
    }
    while(this->selectLastPositionsQuery.next())
    {
        ret->emplace_back(std::make_tuple(this->selectLastPositionsQuery.value(0).toULongLong(),
                                          this->selectLastPositionsQuery.value(1).toDouble(),
                                          this->selectLastPositionsQuery.value(2).toDouble()));
    }
    return ret;
}
std::shared_ptr<std::vector<std::tuple<qulonglong, double, double>>>  QDB::selectAllPositions(qulonglong devid, qulonglong from)
{
    auto ret=std::make_shared<std::vector<std::tuple<qulonglong, double, double>>>();
    this->selectAllPositionsQuery.addBindValue(devid);
    this->selectAllPositionsQuery.addBindValue(from);
    if (!this->selectAllPositionsQuery.exec())
    {
        LOG_FAILED_QUERY(selectAllPositionsQuery);
        return ret;
    }
    while(this->selectAllPositionsQuery.next())
    {
        ret->emplace_back(std::make_tuple(this->selectAllPositionsQuery.value(0).toULongLong(),
                                          this->selectAllPositionsQuery.value(1).toDouble(),
                                          this->selectAllPositionsQuery.value(2).toDouble()));
    }
    return ret;
}

qulonglong QDB::selectSequenceFromId(qulonglong devid)
{
    this->selectSequenceFromIdQuery.addBindValue(devid);
    if (!this->selectSequenceFromIdQuery.exec())
    {
        LOG_FAILED_QUERY(selectSequenceFromIdQuery);
        return 0;
    }
    if(this->selectSequenceFromIdQuery.next())
    {
        return  this->selectSequenceFromIdQuery.value(0).toULongLong();
    }
    return 0;
}

std::shared_ptr<std::vector<std::tuple< qulonglong, qulonglong>>>QDB::selectMostFrequent(qulonglong from)
{
    auto ret=std::make_shared<std::vector<std::tuple<qulonglong, qulonglong>>>();
    this->selectMostFrequentQuery.addBindValue(from);
    this->selectMostFrequentQuery.addBindValue(from);
    if (!this->selectMostFrequentQuery.exec())
    {
        LOG_FAILED_QUERY(selectMostFrequentQuery);
        return ret;
    }
    while(this->selectMostFrequentQuery.next())
    {
        //qDebug()<<"MFQ"<<selectMostFrequentQuery.value(0)<<selectMostFrequentQuery.value(1);
        qulonglong id=this->selectMostFrequentQuery.value(0).toULongLong();
        qulonglong time=this->selectMostFrequentQuery.value(1).toULongLong();
        //qDebug()<<"IIIdDD"<<id;
        //qDebug()<<"TTIME"<<time;
        ret->emplace_back(std::make_tuple(id, time));
    }

    return ret;
}

void QDB::multipleInsert(QList<Packet> & packets){
    this->db.get().transaction();

    try
    {
        qulonglong sequence;
        QSqlQuery q(this->db.get());
        QSqlQuery s(this->db.get());
        q.setForwardOnly(true);
        q.prepare(this->insertQueryString);
        s.setForwardOnly(true);
        s.prepare(this->selectSequenceFromIdQueryString);
        for(auto it=packets.begin(); it!=packets.end(); it++){
            sequence=0;
            s.addBindValue(it->cellId);
            if (!s.exec())
            {
                throw std::exception();
            }
            if(s.next())
            {
                sequence=s.value(0).toULongLong();
            }
            long timeFE=std::chrono::time_point_cast<std::chrono::seconds>(it->time).time_since_epoch().count();
            q.addBindValue(it->hash);
            q.addBindValue(static_cast<qulonglong>(timeFE));
            q.addBindValue(it->x);
            q.addBindValue(it->y);
            q.addBindValue(it->cellId);
            q.addBindValue(it->fakeness);
            q.addBindValue(sequence+1);
            q.addBindValue(it->iteration);
            if(!q.exec())
            {
                throw std::exception();
            }
        }
        this->db.get().commit();
    }
    catch (...) {
      qDebug()<<"InsertFailed";
      this->db.get().rollback();
    }

}

std::tuple<qulonglong, qulonglong, qulonglong> QDB::selectCounts(qulonglong time)
{
    qulonglong allCountN;
    qulonglong fakeCountN;
    qulonglong normalCountN;
    this->db.get().transaction();
    try
    {
        QSqlQuery allCount(this->db.get());
        QSqlQuery fakeCount(this->db.get());
        QSqlQuery normalCount(this->db.get());
        allCount.setForwardOnly(true);
        allCount.prepare(this->selectCountQueryString);
        fakeCount.setForwardOnly(true);
        fakeCount.prepare(this->selectCountWithFakenessQueryString);
        normalCount.setForwardOnly(true);
        normalCount.prepare(this->selectCountWithoutFakenessQueryString);
        allCount.addBindValue(time);
        fakeCount.addBindValue(time);
        normalCount.addBindValue(time);
        if(!allCount.exec())
        {
            throw std::exception();
        }
        if (allCount.next())
        {
            allCountN=allCount.value(0).toULongLong();
        }
        if(!fakeCount.exec())
        {
            throw std::exception();
        }
        if (fakeCount.next())
        {
            fakeCountN=fakeCount.value(0).toULongLong();
        }
        if(!normalCount.exec())
        {
            throw std::exception();
        }
        if (normalCount.next())
        {
            normalCountN=normalCount.value(0).toULongLong();
        }
        this->db.get().commit();
    }
    catch (...)
    {
      this->db.get().rollback();
      allCountN=0;
      fakeCountN=0;
      normalCountN=0;
    }
    return std::make_tuple(allCountN, fakeCountN, normalCountN);
}
