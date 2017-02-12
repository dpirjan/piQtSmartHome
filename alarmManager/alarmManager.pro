QT += core dbus
QT -= gui

TARGET = alarmManager
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

SPEC = $${QMAKESPEC}
message("QMAKESPEC: $${SPEC}")

SYSROOT = $$[QT_SYSROOT]
message("QMAKESYSROOT: $${SYSROOT}")

isEmpty(SYSROOT) {
        message("SYSROOT empty, assuming compiling for host")
}

HEADERS += \
    alarmManager.h

SOURCES += \
    alarmManagerService.cpp \
    alarmManager.cpp

DISTFILES += \
    piHomeAlarm.service \
    piHomeAlarm.conf

unix {
    target.path = /usr/share/pismarthome
    INSTALLS += target

    SYSTEMDSERVICE.files = piHomeAlarm.service
    SYSTEMDSERVICE.path = /lib/systemd/system
    INSTALLS += SYSTEMDSERVICE

    DBUSCONF.files = piHomeAlarm.conf
    DBUSCONF.path = /etc/dbus-1/system.d
    INSTALLS += DBUSCONF
}
