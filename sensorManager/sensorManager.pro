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

contains(SPEC, "raspberry") {
        message("Adding define for wiringPi")
        defines += WIRINGPI
        LIBS += -lwiringPi
}
