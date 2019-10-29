#include "mainwindow.h"
#include <QApplication>
#include <globalstate.h>
#include <threadguard.h>
#include <espserver.h>
#include <QDebug>
#include <functions.h>
#include <stallingstation.h>
int main(int argc, char *argv[])
{
    try {
        GlobalState::getInstance().parse();
        QApplication a(argc, argv);
        QDB::getInstance();
        StallingStation::getInstance();
        EspServer esp;
        esp.startServer();
        MainWindow w;
        w.show();
        return a.exec();

    } catch (std::exception e) {
        qDebug()<<e.what();

    }
    catch(...)
    {

    }


}
