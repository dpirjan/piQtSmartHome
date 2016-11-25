QT += core dbus
QT -= gui

TARGET = userManager
CONFIG += console debug
CONFIG -= app_bundle

TEMPLATE = app

CONFIG(debug, debug|release) {
    message("$${TARGET} - debug mode")
    DEFINES += QT_MESSAGELOGCONTEXT
} else {
    message("$${TARGET} - release mode")
}

INCLUDEPATH += ../piHomeCommon
LIBS += -L../piHomeCommon -lpiHomeCommon

HEADERS += \
    userManager.h \
    userInfo.h \
    simplecrypt.h

SOURCES += \
    userManager.cpp \
    userManagerService.cpp \
    userInfo.cpp \
    simplecrypt.cpp

DISTFILES += \
    piHomeLogin.service \
    piHomeLogin.conf

unix {
    target.path = /usr/share/pismarthome
    INSTALLS += target

    SYSTEMDSERVICE.files = piHomeLogin.service
    SYSTEMDSERVICE.path = /lib/systemd/system
    INSTALLS += SYSTEMDSERVICE

    DBUSCONF.files = piHomeLogin.conf
    DBUSCONF.path = /etc/dbus-1/system.d
    INSTALLS += DBUSCONF
}
