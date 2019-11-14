#include "device.h"

Device::Device()
{

}

void Device::addStation(qulonglong name, int8_t intensty)
{
    auto it=this->stations.find(name);
    if (it!=this->stations.end()){
        it->second.push_back(intensty);
    }
    else {
        QVector<int8_t> v;
        v.push_back(intensty);
        this->stations.emplace(name, v);
    }
}
