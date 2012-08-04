#-------------------------------------------------
#
# Project created by QtCreator 2012-08-02T00:56:07
#
#-------------------------------------------------

QT += core gui
QT += network

TARGET = D2Block
TEMPLATE = app

INCLUDEPATH += h
INCLUDEPATH += src

SOURCES += src/main.cpp\
           src/D2BlockWindow.cpp\
           src/D2BlockApplication.cpp\
           src/D2BlockDownloader.cpp\
           src/D2BlockSettings.cpp\
           src/D2BlockUpdater.cpp

HEADERS += h/D2BlockWindow.h\
           h/D2BlockApplication.h\
           h/D2BlockDownloader.h\
           h/D2BlockSettings.h\
           h/D2BlockUpdater.h

FORMS   += d2block.ui
