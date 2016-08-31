QT += core dbus sql
QT -= gui

TARGET = databaseManager
CONFIG += console debug
CONFIG -= app_bundle

TEMPLATE = app

CONFIG(debug, debug|release) {
    message("$${TARGET} - debug mode")
    DEFINES += QT_MESSAGELOGCONTEXT
} else {
    message("$${TARGET} - release mode")
}

INCLUDEPATH += ../databaseManagerInfo
LIBS += -L../databaseManagerInfo -ldatabaseManagerInfo

INCLUDEPATH += ../../utils
LIBS += -L../../utils -lpiHomeUtils

HEADERS += \
    databaseManager.h

SOURCES += \
    databaseManager.cpp \
    databaseManagerService.cpp

DISTFILES += \
    piHomeDatabase.service \
    piHomeDatabase.conf

unix {
    target.path = /usr/share/pismarthome
    INSTALLS += target

    SYSTEMDSERVICE.files = piHomeDatabase.service
    SYSTEMDSERVICE.path = /lib/systemd/system
    INSTALLS += SYSTEMDSERVICE

    DBUSCONF.files = piHomeDatabase.conf
    DBUSCONF.path = /etc/dbus-1/system.d
    INSTALLS += DBUSCONF
}
