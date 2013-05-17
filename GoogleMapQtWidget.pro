#-------------------------------------------------
#
# Project created by QtCreator 2013-05-16T17:23:45
#
#-------------------------------------------------

QT       += core gui webkit network script
#QT       += debug

TARGET = mgeGoogleMap
TEMPLATE = app


# Working directory
OBJECTS_DIR = $$PWD/build/
MOC_DIR     = $$PWD/build/
RCC_DIR     = $$PWD/build/
UI_DIR      = $$PWD/build/
OUT_PWD     = $$PWD/build/
DESTDIR     = $$PWD/build/


SOURCES += main.cpp\
           mainwindow.cpp

HEADERS  += mainwindow.h


OTHER_FILES += google_maps.html

RESOURCES += resource.qrc
