QT       += core dbus sql
QT       -= gui

TARGET = databaseManager
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += debug

TEMPLATE = app

CONFIG(debug, debug|release) {
    message("$${TARGET} - debug mode")
}else {
    message("$${TARGET} - release mode")
}

DEFINES += QT_MESSAGELOGCONTEXT

INCLUDEPATH += ../databaseManagerInfo
LIBS += -L../databaseManagerInfo -ldatabaseManagerInfo

INCLUDEPATH += ../../utils
LIBS += -L../../utils -lpiHomeUtils

HEADERS += databaseManager.h

SOURCES += databaseManager.cpp \
    databaseManagerService.cpp

unix {
    target.path = /usr/share/pismarthome
    INSTALLS += target
}
