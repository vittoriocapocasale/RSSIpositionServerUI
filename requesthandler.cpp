#include "requesthandler.h"
#include "packet.h"
#include <arpa/inet.h>
#include <QDataStream>
#include <stallingstation.h>
#include <globalstate.h>
RequestHandler::RequestHandler(QBuffer* s){
    this->socket=s;
    connect(this, &RequestHandler::packetsParsed, &StallingStation::getInstance(), &StallingStation::dataHandle);
}

void RequestHandler::run()
{
    bool outcome=true;
    std::vector<Packet> packets;
    //std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<double, std::ratio<1>>>;
    try {
        qulonglong station;
        int8_t intensity;
        auto s=this->socket;
        qulonglong iteration=GlobalState::getInstance().getIteration();
        // qDebug()<<"AAA";
        if (s->bytesAvailable()>=10)
        {
            station=std::stoul(s->read(6).toHex().toStdString(), nullptr, 16);
            uint32_t packetsCount=ntohl(*reinterpret_cast<uint32_t*>(s->read(4).data()));
            for(unsigned int i=0; i<packetsCount; i++)
            {
                if(s->bytesAvailable()>=1)
                {
                     //qDebug()<<"CCC";
                    Packet p;
                    uint8_t size=0;
                    size=*reinterpret_cast<uint8_t*>(s->read(1).data());
                    if(s->bytesAvailable()>=size)
                    {
                        //qDebug()<<"DDD";
                        p.hash=ntohll(*reinterpret_cast<uint64_t*>(s->read(8).data()));
                        uint64_t temp=*(reinterpret_cast<uint64_t*>(s->read(8).data()));
                        p.cellId=ntohll(temp);
                        //temp=(temp&0x0000400000000000)>>62;
                        //qDebug()<<"temp1"<<temp;
                        temp=(p.cellId&0x0000400000000000);
                        //qDebug()<<"temp2"<<temp;
                        if(temp>0)
                        {
                            p.fake=true;
                        }
                        else
                        {
                            p.fake=false;
                        }
                        // qDebug()<<"EEE";
                        intensity=*reinterpret_cast<int8_t*>(s->read(1).data());
                        //qDebug()<<"Intensity "<<intensity<<p.cellId;
                        p.addDevice(station, intensity);
                        // p.time=
                        ntohl(*(reinterpret_cast<uint8_t*>(s->read(4).data())));
                        uint8_t l=*reinterpret_cast<uint8_t*>(s->read(1).data());
                        p.ssid=s->read(l);
                        p.iteration=iteration;
                        packets.push_back(std::move(p));

                    }
                    else {
                        outcome=false;
                    }
                }
                else {
                    outcome=false;
                }
            }
            if(s->bytesAvailable()>0)
            {
                outcome=false;
            }
            if(outcome)
            {
                StallingStation::getInstance().addPackets(packets, GlobalState::getInstance().getTime());
            }
            std::unique_lock<std::mutex> ul(GlobalState::getInstance().espMutex);
            while (GlobalState::getInstance().espStatus==1)
            {
                GlobalState::getInstance().espCv.wait(ul);
            }
            GlobalState::getInstance().espCount++;
            if(GlobalState::getInstance().espCount==GlobalState::getInstance().getStations().size())
            {
                emit packetsParsed(GlobalState::getInstance().getTime().time_since_epoch().count());
                GlobalState::getInstance().espCount--;
                GlobalState::getInstance().randomizeChannel();
                GlobalState::getInstance().updateTime();
                GlobalState::getInstance().espStatus=1;
                GlobalState::getInstance().espCv.notify_all();
            }
            else
            {
                while (GlobalState::getInstance().espStatus==0)
                {
                    std::cv_status stat=GlobalState::getInstance().espCv.wait_for(ul, std::chrono::seconds(20));
                    if(stat==std::cv_status::timeout)
                    {
                        QByteArray b;
                        b.push_back('W');
                        b.push_back('\0');
                        emit bufferReady( b);
                    }
                }
                GlobalState::getInstance().espCount--;
            }
            if(GlobalState::getInstance().espCount==0)
            {
                GlobalState::getInstance().espStatus=0;
                GlobalState::getInstance().espCv.notify_all();
            }
            uint8_t chan= GlobalState::getInstance().getChannel();
            ul.unlock();
            QByteArray b;
            b.push_back('C');
            b.push_back(static_cast<char>(chan));
            //qDebug()<<"Channel"<<uint(chan);
            qDebug()<<station<<packetsCount;
            //s->write(b);
            emit bufferReady( b);
        }
        else {
            outcome=false;
        }

    }
    catch (std::exception &e)
    {
        qDebug()<<e.what();
        outcome=false;
    }
    catch (...) {
        qDebug()<<"Something wrong";
        outcome=false;
    }
    emit  success(outcome);
    //qDebug()<<outcome;
}
