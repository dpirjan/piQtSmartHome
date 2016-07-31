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

ARCH = $$QMAKE_HOST.arch
equals(ARCH, "x86_64") {
    message("PC 64 bit")
    DEFINES += NO_WIRING_PI
}
equals(ARCH, "x86") {
    message("PC 32 bit")
    DEFINES += NO_WIRING_PI
}
