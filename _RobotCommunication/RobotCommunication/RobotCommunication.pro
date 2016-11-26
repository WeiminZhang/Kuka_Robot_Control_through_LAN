#-------------------------------------------------
#
# Project created by QtCreator 2016-11-14T11:26:53
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RobotCommunication
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpnetworkserver.cpp \
    subject.cpp \
    modulenetwork.cpp \
    savecustomsettings.cpp \
    xmlparsing.cpp \
    algorithmescan.cpp

HEADERS  += mainwindow.h \
    tcpnetworkserver.h \
    hostaddressconfig.h \
    messagebox.h \
    observer.h \
    event.h \
    subject.h \
    eventcollection.h \
    modulenetwork.h \
    eventnetwork.h \
    statenetwork.h \
    savecustomsettings.h \
    xmlparsing.h \
    robotdata.h \
    eventrobotdata.h \
    algorithmescan.h \
    eventalgorithm1.h

FORMS    += mainwindow.ui
