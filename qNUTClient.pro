#-------------------------------------------------
#
# Project created by QtCreator 2011-05-15T17:10:35
#
#-------------------------------------------------

QT       += core gui network

TARGET = qNUTClient
TEMPLATE = app

QMAKE_CXXFLAGS_DEBUG += -D DEBUG

SOURCES += main.cpp\
        mainwindow.cpp \
    upsclient.cpp

HEADERS  += mainwindow.h \
    upsclient.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    VAR_LIST.txt
