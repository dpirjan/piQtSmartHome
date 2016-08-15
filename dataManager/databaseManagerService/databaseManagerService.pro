QT       += core dbus sql
QT       -= gui

TARGET = databaseManager
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../databaseManagerInfo
LIBS += -L../databaseManagerInfo -ldatabaseManagerInfo

HEADERS += databaseManager.h \
    settingsManagerInterface.h

SOURCES += databaseManager.cpp \
    settingsManagerInterface.cpp \
    databaseManagerService.cpp
