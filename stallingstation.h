#ifndef STALLINGSTATION_H
#define STALLINGSTATION_H
#include <map>
#include <packet.h>
#include <mutex>
#include <QObject>
#include <QTimer>
#include <globalstate.h>
#include <random>
#include <qdb.h>
#include <unordered_set>
#include <chrono>
#include <device.h>
#include <dbwriter.h>
class StallingStation:public QObject
{
    using timeDuration=std::chrono::duration<qulonglong, std::ratio<1>>;
    using timeTimePoint= std::chrono::time_point<std::chrono::system_clock, timeDuration>;
     Q_OBJECT
public:
    static StallingStation& getInstance(){
        static StallingStation s;
        return s;
    }
    StallingStation(StallingStation &)=delete;
    StallingStation & operator=(StallingStation &)=delete;
    void addPackets(std::vector<Packet> pkts, timeTimePoint t);
public slots:
    void dataHandle(qulonglong time);
    void databaseUpdated(qulonglong time);
signals:
    void newDataAvailable(qulonglong time);
private:
    StallingStation();
    void addPacket(Packet pkt, timeTimePoint t);
    std::pair<double, double> intersectionFinder(double x1, double b_y1, double r1, double x2, double y2,double r2, double x3, double y3, double r3);
    QMap<uint64_t, Device> devices;
    QMap<uint64_t, timeTimePoint> fakes;


};

#endif // STALLINGSTATION_H
