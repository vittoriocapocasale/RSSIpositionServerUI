#ifndef PACKET_H
#define PACKET_H
#include <qstring.h>
#include <unordered_map>
#include <map>
#include <chrono>
#include <qdebug.h>
class Packet
{
public:
    using timepoint=std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<qulonglong, std::ratio<1>>>;
    Packet();
    Packet(quint64 hash, timepoint time, QString ssid, quint64 cellId);
    void addDevice(qulonglong name, int8_t intensty);
    void mergeDevices(Packet& p);
    quint64 hash;
    //int8_t intensity;
    std::map<qulonglong, int8_t> devices;
    timepoint time;
    QString ssid;
    quint64 cellId;
    bool fake;
    qulonglong iteration;
    double x;
    double y;
    int fakeness;
};

#endif // PACKET_H
