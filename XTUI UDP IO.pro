#-------------------------------------------------
#
# Project created by QtCreator 2013-10-16T12:49:50
#
#-------------------------------------------------

QT       += core gui network

CONFIG += console # use -d option when launcing application, see main.cpp

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XTUI_UDP_IO
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    receivethread.cpp \
    output.cpp \
    outputdlg.cpp \
    outputv2.cpp \
    inputdlg.cpp

HEADERS  += mainwindow.h \
    receivethread.h \
    output.h \
    outputdlg.h \
    outputv2.h \
    inputdlg.h \
    xp2defs.h

FORMS    += mainwindow.ui \
    output.ui \
    outputdlg.ui \
    outputv2.ui \
    inputdlg.ui

OTHER_FILES +=

RESOURCES += \
    Resource.qrc

RC_FILE += \
    icon.rc

DISTFILES += \
    outputSlider.stylesheet


