#include "stallingstation.h"

StallingStation::StallingStation()
{

}
void StallingStation::addPacket(Packet pkt, timeTimePoint t)
{
    auto it=this->devices.find(pkt.cellId);
    if(it!=this->devices.end())
    {
        it->addStation(pkt.station, pkt.intensity);
    }
    else
    {
        Device d;
        d.x=-2;
        d.y=-2;
        d.addStation(pkt.station,pkt.intensity);
        d.fake=pkt.fake;
        d.time=t;
        d.devId=pkt.cellId;
        this->devices.insert(pkt.cellId, d);
    }

}

void StallingStation::addPackets(std::vector<Packet> pkts, timeTimePoint t)
{
    std::lock_guard<std::mutex> l(GlobalState::getInstance().packetsMutex);
    for(auto i:pkts)
    {
        this->addPacket(i, t);
    }
}

void StallingStation::dataHandle(qulonglong time)
{

    //qDebug()<<"called";
    try {
        DBWriter *rh = new DBWriter(this->devices, this->fakes, time);
        connect(rh, &DBWriter::databaseUpdated, this, &StallingStation::databaseUpdated);
        QThreadPool::globalInstance()->start(rh);
    }
    catch(...)
    {
        qDebug()<<"badError StallingStation";
    }
}

void StallingStation::databaseUpdated(qulonglong time)
{
    emit newDataAvailable(time);
}

