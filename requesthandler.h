#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include <QRunnable>
#include <QBuffer>
#include <memory>
#include <functions.h>
#include <QThreadPool>
class RequestHandler : public QObject, public QRunnable
{
    Q_OBJECT
public:
    RequestHandler(QBuffer* s);
signals:
    void success(bool outcome);
    void bufferReady (QByteArray b);
    void packetsParsed(qulonglong time);
protected:
    void run();
private:
    QBuffer* socket;
};

#endif // REQUESTHANDLER_H
