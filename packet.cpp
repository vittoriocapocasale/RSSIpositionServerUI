#include "packet.h"
Packet::Packet () {}

Packet::Packet(quint64 hash, timepoint time, QString ssid, quint64 cellId):
    hash(hash), time(time), ssid(ssid), cellId(cellId){}
/*
void Packet::addDevice(qulonglong name, int8_t intensty)
{
    if( this->devices.find(name)!=this->devices.end())
    {
        qDebug()<<"double";
        return;
    }
    this->devices.emplace(name, intensty);
}
void Packet::mergeDevices(Packet& p)
{
    for (auto it= p.devices.begin(); it!=p.devices.end(); it++)
    {
        if(this->devices.find(it->first)==this->devices.end())
        {
            this->devices.emplace(it->first, it->second);
        }
    }
}
*/
