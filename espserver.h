#ifndef ESPSERVER_H
#define ESPSERVER_H

#include <QObject>

class EspServer : public QTcpServer
{
public:
    EspServer();
};

#endif // ESPSERVER_H
