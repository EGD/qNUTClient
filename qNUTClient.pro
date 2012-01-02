#-------------------------------------------------
#
# Project created by QtCreator 2011-05-15T17:10:35
#
#-------------------------------------------------

QT       += core gui network

TARGET = qNUTClient
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    upsclient.cpp \
    qupsdock.cpp

HEADERS  += mainwindow.h \
    upsclient.h \
    qupsdock.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    VAR_LIST.txt

RESOURCES += \
    resource.qrc

