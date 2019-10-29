#include "tcpconnection.h"

TcpConnection::TcpConnection(qintptr sd, QObject *parent)
{
    this->s=new QTcpSocket(this);
    this->b.open(QBuffer::ReadWrite);
    connect(s, SIGNAL(connected()), this, SLOT(connected()));
    connect(s, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(s, SIGNAL(readyRead()), this, SLOT(readyRead()));
    s->setSocketDescriptor(sd);
    this->firstRead=true;
}

TcpConnection::~TcpConnection()
{
    this->b.close();
}

void TcpConnection::connected()
{

}
void TcpConnection::disconnected()
{

}
void TcpConnection::readyRead()
{
    if (firstRead && s->bytesAvailable()<4)
    {
        return;
    }
    else if (firstRead)
    {
        uint32_t ps=ntohl(*reinterpret_cast<uint32_t*>(s->read(4).data()));
        this->bytesLeft=0+ps;
        this->b.buffer().append(s->readAll());
        this->bytesLeft-=b.bytesAvailable();
        firstRead=false;
    }
    else {
        this->bytesLeft-=s->bytesAvailable();
        this->b.buffer().append(s->readAll());
    }
    //qDebug()<<"Size:"<<this->bytesLeft;
    if(this->bytesLeft<=0)
    {
        RequestHandler *rh = new RequestHandler(&b);
        rh->setAutoDelete(true);
        connect (rh, &RequestHandler::bufferReady, this, &TcpConnection::writeToClient, Qt::ConnectionType::QueuedConnection);
        //connect(rh, &RequestHandler::success, this, &TcpConnection::closedConnection, Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(rh);
    }
}
void TcpConnection::writeToClient(QByteArray b)
{
    try {
        this->s->write(b);
    } catch (...) {
        qDebug()<<"badError TCPConnection Write to client";
    }

}

void TcpConnection::closedConnection(bool b)
{
    try {
        this->b.close();
    } catch (...) {
        qDebug()<<"badError TCPConnection closedConnection";
    }

}
