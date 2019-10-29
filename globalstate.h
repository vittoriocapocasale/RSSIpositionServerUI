#ifndef GLOBALSTATE_H
#define GLOBALSTATE_H
#include <map>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <QFile>
#include <QTextStream>
#include <exception>
#include <random>

class GlobalState
{
public:
    using timePoint = std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<qulonglong, std::ratio<1>>>;
    static GlobalState & getInstance(){
        static GlobalState gs;
        return gs;
    }
    std::mutex packetsMutex;
    std::mutex espMutex;
    std::condition_variable espCv;
    volatile int espCount;
    volatile int espStatus;
    GlobalState(GlobalState &)= delete ;
    GlobalState & operator=(GlobalState &)=delete;
    const QHash<qulonglong, std::pair<double, double>> & getStations();
    const timePoint & getTime();
    void updateTime();
    unsigned long getIteration();
    unsigned short getPort();
    void parse();
    void randomizeChannel();
    double getScale();
    int8_t getCoefficientA();
    double getCoefficientN();
    uint8_t getChannel();
private:
    GlobalState();
    QHash<qulonglong, std::pair<double, double>> stations;
    timePoint time;
    unsigned long iteration;
    std::mutex timeMutex;
    unsigned short port;
    uint8_t channel;
    std::minstd_rand randomEngine;
    std::uniform_int_distribution<> randomDistribution;
    int8_t coefficientA;
    double coefficientN;
    double scale;

};

#endif // GLOBALSTATE_H
