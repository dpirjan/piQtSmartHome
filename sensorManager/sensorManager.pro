QT       += core dbus
QT       -= gui

TARGET = sensorManager
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

INCLUDEPATH += ../dataManager/databaseManagerInfo
LIBS += -L../dataManager/databaseManagerInfo -ldatabaseManagerInfo

INCLUDEPATH += ../utils
LIBS += -L../utils -lpiHomeUtils

SOURCES += \
    actuator.cpp \
    sensor.cpp \
    wiredSensor.cpp \
    wirelessSensor.cpp \
    actuatorInstantiator.cpp \
    sensorInstantiator.cpp \
    databaseManagerInterface.cpp \
    sensorManagerService.cpp \
    mailManagerInterface.cpp


HEADERS += \
    common.h \
    actuator.h \
    sensor.h \
    wiredSensor.h \
    wirelessSensor.h \
    actuatorInstantiator.h \
    sensorInstantiator.h \
    databaseManagerInterface.h \
    mailManagerInterface.h


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

unix {
    target.path = /usr/share/pismarthome
    INSTALLS += target
}
