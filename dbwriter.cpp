#include "dbwriter.h"

DBWriter::DBWriter(QMap<uint64_t, Packet>& newPkts, QMap<uint64_t, timeTimePoint>& fakeAddresses, qulonglong time):
    newPackets(newPkts), fakes(fakeAddresses)
{
    std::chrono::duration<qulonglong> dur(time);
    timeTimePoint t(dur);
    this->time=t;
}

void DBWriter::run()
{

    try {
        QHash<uint64_t, Packet> dbPackets;
        std::vector<unsigned int> seq;
        auto scale= GlobalState::getInstance().getScale();
        int8_t coefficientA=GlobalState::getInstance().getCoefficientA();
        double coefficientN= GlobalState::getInstance().getCoefficientN();
        std::lock_guard<std::mutex> lg(GlobalState::getInstance().packetsMutex);
        for (auto i=this->newPackets.begin(); i!=this->newPackets.end();)
        {
            double x=-2;
            double y=-2;
            if (i->devices.size()>=3)
            {
                std::vector<double> intensities;
                std::vector<double> xValues;
                std::vector<double> yValues;
                for (auto j=i->devices.begin(); j!= i->devices.end() && intensities.size()<3;j++)
                {

                    xValues.push_back(GlobalState::getInstance().getStations().find(j->first)->first);
                    yValues.push_back(GlobalState::getInstance().getStations().find(j->first)->second);
                    intensities.push_back(pow(10,((coefficientA-j->second)/(10*coefficientN)))/scale);
                }
                qDebug()<<xValues[0]<< yValues[0]<< intensities[0];
                qDebug()<<xValues[1]<< yValues[1]<< intensities[1];
                qDebug()<<xValues[2]<< yValues[2]<< intensities[2];
                //qDebug()<<i->devices;
                auto coordinates= intersectionFinder(xValues[0], yValues[0], intensities[0],
                                                     xValues[1], yValues[1], intensities[1],
                                                     xValues[2], yValues[2], intensities[2]);
                x=coordinates.first;
                y=coordinates.second;
                qDebug()<<"XY"<<x<<y;
                if(!i->fake)
                {
                    i->fakeness=0;
                }
                else
                {
                    i->fakeness=2;
                }
            }
            if(x>-1&&x<1&&y>-1&&y<1)
            {
                i->x=x;
                i->y=y;
                i++;
            }
            else
            {
                i=newPackets.erase(i);
            }
        }
        for(auto i=this->newPackets.begin();i!=this->newPackets.end(); i++)
        {
            auto j= fakes.find(i->hash);
            if(j!= fakes.end()&&i->fakeness==2)
            {
                i->fakeness=1;
            }
            if(i->fakeness==2)
            {
                for (auto j=this->newPackets.begin(); j!=this->newPackets.end();j++)
                {
                    if(j->fakeness!=2&&j!=i&&(((j->x-i->x)*(j->x-i->x)+(j->y-i->y)*(j->y-i->y))<0.001||j->hash==i->hash))
                    {
                        i->fakeness=1;
                        this->fakes.insert(i->hash, i->time);
                    }
                }
            }
            auto p=dbPackets.find(i->cellId);
            {
                if(p==dbPackets.end()||p->fakeness>i->fakeness)
                {
                    dbPackets.insert(i->cellId,*i);
                }
            }
        }
        this->newPackets.clear();
        for(auto it=fakes.begin(); it!= fakes.end();)
        {
            qulonglong elapsedTime = std::chrono::duration<qulonglong, std::ratio<1>>(time-*it).count();
            if(elapsedTime>360)
            {
                it=fakes.erase(it);
            }
            else{
                it++;
            }
        }
        QList<Packet> pks= dbPackets.values();
        QDB::getInstance().multipleInsert(pks);
        emit databaseUpdated(time.time_since_epoch().count());
    }
    catch (...)
    {
        qDebug()<<"BadError DBWriter";
    }
}
std::pair<double, double> DBWriter::intersectionFinder(double x1, double b_y1, double r1, double x2, double y2,double r2, double x3, double y3, double r3)
{
    double d2_tmp_idx_1;
    double d2_tmp_idx_0;
    double T_idx_0;
    double d2;
    double a;
    double P1_idx_0;
    double P1_idx_1;
    double t;
    double P2_idx_0;
    d2_tmp_idx_1 = x2 - x1;
    d2_tmp_idx_0 = d2_tmp_idx_1;
    T_idx_0 = d2_tmp_idx_1 * d2_tmp_idx_1;
    d2_tmp_idx_1 = y2 - b_y1;
    d2 = T_idx_0 + d2_tmp_idx_1 * d2_tmp_idx_1;
    a = (r1 * r1 - r2 * r2) / d2 / 2.0;
    P1_idx_0 = (x1 + x2) / 2.0 + a * d2_tmp_idx_0;
    P1_idx_1 = (b_y1 + y2) / 2.0 + a * d2_tmp_idx_1;
    a = r1 + r2;
    T_idx_0 = r2 - r1;
    t = (a * a - d2) * (d2 - T_idx_0 * T_idx_0);
    if (t >= 0.0) {
        a = std::sqrt(t) / d2 / 2.0;
        d2 = a * 0.0 * d2_tmp_idx_0 + -a * d2_tmp_idx_1;
        P2_idx_0 = P1_idx_0 + d2;
        P1_idx_0 -= d2;
        d2 = a * d2_tmp_idx_0 + a * 0.0 * d2_tmp_idx_1;
        T_idx_0 = P1_idx_1 + d2;
        P1_idx_1 -= d2;
        if(t==0.0)
        {
            return std::make_pair(P2_idx_0, T_idx_0);
        }
        else
        {
            double distance1=std::fabs(std::sqrt((P2_idx_0-x3)*(P2_idx_0-x3)+(T_idx_0-y3)*(T_idx_0-y3))-r3);
            double distance2=std::fabs(std::sqrt((P1_idx_0-x3)*(P1_idx_0-x3)+(P1_idx_1-y3)*(P1_idx_1-y3))-r3);
            if (distance1<distance2)
            {
                return std::make_pair(P2_idx_0, T_idx_0);
            }
            else
            {
                return std::make_pair(P1_idx_0, P1_idx_1);
            }
        }

    }
    return std::make_pair(-3,-3);
}
