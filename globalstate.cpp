#include "globalstate.h"
#include "QDebug"
#include "QCoreApplication"
GlobalState::GlobalState():randomEngine(0), randomDistribution(1,13)
{
    this->updateTime();
    this->espCount=0;
    this->iteration=0;
    this->espStatus=0;
    this->scale=1;
}
const QHash<qulonglong, std::pair<double, double>>  &
GlobalState::getStations()
{
    return this->stations;
}

const std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<qulonglong, std::ratio<1>>> & GlobalState::getTime()
{
    std::lock_guard<std::mutex> l(this->timeMutex);
    return this->time;
}
void GlobalState::updateTime()
{
    std::lock_guard<std::mutex> l(this->timeMutex);
    this->iteration++;
    this->time=std::chrono::time_point_cast<std::chrono::duration<qulonglong, std::ratio<1>>> (std::chrono::system_clock::now());
}
unsigned long GlobalState::getIteration()
{
    return this->iteration;
}
unsigned short GlobalState::getPort()
{
    return this->port;
}
void  GlobalState::randomizeChannel()
{
    //this->channel=this->randomDistribution(this->randomEngine);
}
uint8_t GlobalState::getChannel()
{
    return this->channel;
}
int8_t GlobalState::getCoefficientA()
{
    return this->coefficientA;
}
double GlobalState::getCoefficientN()
{
    return this->coefficientN;
}

double GlobalState::getScale()
{
    return this->scale;
}
void GlobalState::parse()
{
    QFile file(":/config.txt");
    QVector<QString> strings;
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<file.fileName();
        qDebug()<<file.errorString();
        throw std::runtime_error("Configuration file not found");
    }
    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        if(!line.startsWith("#")&&!line.isEmpty())
        {
            strings.push_back(line);
        }
    }
    this->port= strings[0].toUShort();
    this->channel= static_cast<uint8_t>(strings[1].toUShort());
    this->coefficientA= static_cast<int8_t>(strings[2].toShort())*(-1);
    this->coefficientN= strings[3].toDouble();
    this->scale= strings[4].toDouble();
    qDebug()<<this->port;
    for (int i=5; i<strings.size(); i++)
    {
        QStringList fields = strings[i].split(",");
        this->stations.insert(std::stoul (fields[0].toStdString(), nullptr,16),(std::make_pair(fields[1].toDouble(), fields[2].toDouble())));
    }
    file.close();
}
