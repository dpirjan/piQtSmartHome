QT += core dbus network
QT -= gui

TARGET = mailManager
CONFIG += console debug
CONFIG -= app_bundle

TEMPLATE = app

CONFIG(debug, debug|release) {
    message("$${TARGET} - debug mode")
    DEFINES += QT_MESSAGELOGCONTEXT
} else {
    message("$${TARGET} - release mode")
}

INCLUDEPATH += ../../piHomeCommon
LIBS += -L../../piHomeCommon -lpiHomeCommon

HEADERS += \
    mailManager.h

SOURCES += \
    mailManager.cpp \
    mailManagerService.cpp

DISTFILES += \
    piHomeMail.service \
    piHomeMail.conf

unix {
    target.path = /usr/share/pismarthome
    INSTALLS += target

    SYSTEMDSERVICE.files = piHomeMail.service
    SYSTEMDSERVICE.path = /lib/systemd/system
    INSTALLS += SYSTEMDSERVICE

    DBUSCONF.files = piHomeMail.conf
    DBUSCONF.path = /etc/dbus-1/system.d
    INSTALLS += DBUSCONF
}
