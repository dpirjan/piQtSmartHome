QT += core dbus
QT -= gui

TARGET = sensorManager
CONFIG += console debug
CONFIG -= app_bundle

TEMPLATE = app

CONFIG(debug, debug|release) {
    message("$${TARGET} - debug mode")
    DEFINES += QT_MESSAGELOGCONTEXT
} else {
    message("$${TARGET} - release mode")
}

INCLUDEPATH += ../dataManager/databaseManagerInfo
LIBS += -L../dataManager/databaseManagerInfo -ldatabaseManagerInfo

INCLUDEPATH += ../utils
LIBS += -L../utils -lpiHomeUtils

SPEC = $${QMAKESPEC}
message("QMAKESPEC: $${SPEC}")

SYSROOT = $$[QT_SYSROOT]
message("QMAKESYSROOT: $${SYSROOT}")

isEmpty(SYSROOT) {
        message("SYSROOT empty, assuming compiling for host")
}

!isEmpty(SYSROOT) {
        message("SYSROOT not empty, assuming compiling for target")
        DEFINES += WIRINGPI
        LIBS += -lwiringPi
        message("$${LIBS}")
}

HEADERS += \
    common.h \
    actuator.h \
    sensor.h \
    wiredSensor.h \
    wirelessSensor.h \
    actuatorInstantiator.h \
    sensorInstantiator.h \
    databaseManagerInterface.h \
    mailManagerInterface.h \
    sensorManager.h

SOURCES += \
    actuator.cpp \
    sensor.cpp \
    wiredSensor.cpp \
    wirelessSensor.cpp \
    actuatorInstantiator.cpp \
    sensorInstantiator.cpp \
    databaseManagerInterface.cpp \
    sensorManagerService.cpp \
    mailManagerInterface.cpp \
    sensorManager.cpp

DISTFILES += \
    piHomeSensor.service

unix {
    target.path = /usr/share/pismarthome
    INSTALLS += target

    SYSTEMDSERVICE.files = piHomeSensor.service
    SYSTEMDSERVICE.path = /lib/systemd/system
    INSTALLS += SYSTEMDSERVICE
}
