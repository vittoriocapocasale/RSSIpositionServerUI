#ifndef DBWRITER_H
#define DBWRITER_H

#include <QObject>
#include <QRunnable>
#include <packet.h>
#include <device.h>
#include <memory>
#include <mutex>
#include <QMap>
#include <globalstate.h>
#include <qdb.h>
#include <math.h>
class DBWriter: public QObject, public QRunnable
{
    Q_OBJECT
    using timeDuration=std::chrono::duration<qulonglong, std::ratio<1>>;
    using timeTimePoint= std::chrono::time_point<std::chrono::system_clock, timeDuration>;

public:
    DBWriter(QMap<uint64_t, Device>& devices, QMap<uint64_t, timeTimePoint>& knownFake, qulonglong time);

signals:
    void databaseUpdated(qulonglong time);
protected:
    void run();
private:
    std::pair<double, double> intersectionFinder(double x1, double b_y1, double r1, double x2, double y2,double r2, double x3, double y3, double r3);
    std::pair<double,double> minMax(std::vector<double>& x, std::vector<double>& y, std::vector<double>& r);
    QMap<uint64_t, Device> & devices;
    QMap<uint64_t, timeTimePoint> & fakes;
    timeTimePoint time;
};

#endif // DBWRITER_H
