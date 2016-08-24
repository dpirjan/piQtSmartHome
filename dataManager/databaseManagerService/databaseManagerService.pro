QT       += core dbus sql
QT       -= gui

TARGET = databaseManager
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../databaseManagerInfo
LIBS += -L../databaseManagerInfo -ldatabaseManagerInfo

HEADERS += databaseManager.h

SOURCES += databaseManager.cpp \
    databaseManagerService.cpp

target.path = /usr/share/pismarthome
INSTALLS += target
