#include "stallingstation.h"

StallingStation::StallingStation()
{

}
void StallingStation::addPacket(Packet pkt, timeTimePoint t)
{
    auto it=this->newPackets.find(pkt.hash);
    if(it!=this->newPackets.end())
    {
        it->mergeDevices(pkt);
        //qDebug()<<"pkts"<<it->second.devices.size();
    }
    else
    {
        pkt.time=t;
        pkt.x=-2;
        pkt.y=-2;
        this->newPackets.insert(pkt.hash, pkt);
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
        DBWriter *rh = new DBWriter(this->newPackets, this->fakes, time);
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

