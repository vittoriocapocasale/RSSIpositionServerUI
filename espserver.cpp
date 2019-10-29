#include "espserver.h"
#include <memory>
EspServer::EspServer(QObject *parent) :
    QTcpServer(parent)
{
}

EspServer::EspServer()
{
}
void EspServer::startServer()
{
    QThreadPool::globalInstance()->setExpiryTimeout(300000);
    QThreadPool::globalInstance()->setMaxThreadCount(static_cast<int>(GlobalState::getInstance().getStations().size())+2);
    auto a= listen(QHostAddress::Any, GlobalState::getInstance().getPort());
    if(!a)
    {
        throw new std::runtime_error("Error in server listen");
    }
}

void EspServer::incomingConnection(qintptr socketDescriptor)
{
    try {
        new TcpConnection(socketDescriptor, this);

    } catch (std::exception e) {
        qDebug()<<e.what();
    }

}


