#ifndef DEVICE_H
#define DEVICE_H

#include <qstring.h>
#include <unordered_map>
#include <map>
#include <chrono>
#include <qdebug.h>

class Device
{
public:
    using timepoint=std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<qulonglong, std::ratio<1>>>;
    Device();
    void addStation(qulonglong name, int8_t intensty);
    //int8_t intensity;
    std::map<qulonglong, QVector<int8_t>> stations;
    timepoint time;
    quint64 devId;
    bool fake;
    double x;
    double y;
    int fakeness;
};

#endif // DEVICE_H
