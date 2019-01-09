#-------------------------------------------------
#
# Project created by QtCreator 2018-10-08T09:32:03
#
#-------------------------------------------------

QT       += core gui charts serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = PPP
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    src/ephemeris.c \
    src/geoid.c \
    src/lambda.c \
    src/mymath.c \
    src/options.c \
    src/pntpos.c \
    src/ppp.c \
    src/ppp_ar.c \
    src/ppp_prepare.c \
    src/preceph.c \
    src/rinex.c \
    src/rtcm.c \
    src/rtcm3.c \
    src/rtkcmn.c \
    src/rtkpos.c \
    src/sbas.c \
    src/solution.c \
    src/spp.c \
    src/start.c \
    src/stream.c \
    src/tides.c \
    qcustomplot.cpp \
    dialogconfig.cpp \
    serial.cpp \
    tcpconfig.cpp

HEADERS += \
        mainwindow.h \
    src/rtklib.h \
    qcustomplot.h \
    dialogconfig.h \
    serial.h \
    tcpconfig.h

FORMS += \
        mainwindow.ui \
    dialogconfig.ui \
    serial.ui \
    tcpconfig.ui

