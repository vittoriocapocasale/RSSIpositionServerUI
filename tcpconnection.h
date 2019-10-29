#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
#include <QObject>
#include <QTcpSocket>
#include <QBuffer>
#include <requesthandler.h>
#include <arpa/inet.h>
class TcpConnection: public QObject
{
    Q_OBJECT
public:
    explicit TcpConnection(qintptr sd, QObject *parent = 0);
    ~TcpConnection();
    TcpConnection& operator=(TcpConnection &)= delete;
    TcpConnection(TcpConnection&)=delete;
public slots:
    void connected();
    void disconnected();
    void readyRead();
    void writeToClient(QByteArray b);
    void closedConnection(bool b);

private:
    QTcpSocket* s;
    QBuffer b;
    bool firstRead;
    int64_t bytesLeft;
};

#endif // TCPCONNECTION_H
