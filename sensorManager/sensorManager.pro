QT       += core dbus
QT       -= gui

TARGET = sensorManager
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../databaseManager/databaseManagerInfo
LIBS += -L../databaseManager/databaseManagerInfo -ldatabaseManagerInfo

SOURCES += \
    sensor.cpp \
    actuator.cpp \
    sensorInstantiator.cpp \
    actuatorInstantiator.cpp \
    databaseManagerInterface.cpp \
    sensorManagerService.cpp

HEADERS += \
    sensor.h \
    actuator.h \
    common.h \
    sensorInstantiator.h \
    actuatorInstantiator.h \
    databaseManagerInterface.h

SPEC = $${QMAKESPEC}
message("QMAKESPEC: $${SPEC}")

SYSROOT = $$[QT_SYSROOT]
message("QMAKESYSROOT: $${SYSROOT}")

isEmpty(SYSROOT) {
        message("SYSROOT empty, assuming compiling for host")
}

!isEmpty(SYSROOT) {
        message("SYSROOT not empty, assuming compiling for target")
        message("Adding define for wiringPi based on sysroot")
        DEFINES += WIRINGPI
        LIBS += -lwiringPi
        message("$${LIBS}")
}