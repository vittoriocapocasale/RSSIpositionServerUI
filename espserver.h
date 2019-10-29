#ifndef ESPSERVER_H
#define ESPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QThreadPool>
#include <globalstate.h>
#include <requesthandler.h>
#include <map>
#include<QBuffer>
#include <tcpconnection.h>
class EspServer : public QTcpServer
{
    Q_OBJECT
public:
    EspServer(QObject *parent);
    EspServer();
    void startServer();
protected:
     void incomingConnection(qintptr socketDescriptor);
};

#endif // ESPSERVER_H
