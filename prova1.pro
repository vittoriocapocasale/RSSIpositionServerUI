#-------------------------------------------------
#
# Project created by QtCreator 2019-05-16T18:36:59
#
#-------------------------------------------------

QT       += core gui charts sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport network

TARGET = prova1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        chart.cpp \
        currentpositionshower.cpp \
        custompoint.cpp \
        dbwriter.cpp \
        device.cpp \
        espserver.cpp \
        functions.cpp \
        globalstate.cpp \
        historyshower.cpp \
        main.cpp \
        mainwindow.cpp \
        movementsshower.cpp \
        packet.cpp \
        qcustomplot.cpp \
        qdb.cpp \
        qlistwidgetextended.cpp \
        requesthandler.cpp \
        sldb.cpp \
        stallingstation.cpp \
        tcpconnection.cpp \
        threadguard.cpp

HEADERS += \
        chart.h \
        circularbuffer.h \
        currentpositionshower.h \
        custompoint.h \
        dbwriter.h \
        device.h \
        espserver.h \
        functions.h \
        globalstate.h \
        historyshower.h \
        mainwindow.h \
        movementsshower.h \
        packet.h \
        qcustomplot.h \
        qdb.h \
        qlistwidgetextended.h \
        requesthandler.h \
        sldb.h \
        stallingstation.h \
        tcpconnection.h \
        threadguard.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    config.txt \
    sqlDb \
    sqlDb2

RESOURCES += \
    resources.qrc
