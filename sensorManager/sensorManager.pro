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

INCLUDEPATH += ../piHomeCommon
LIBS += -L../piHomeCommon -lpiHomeCommon

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
    actuator.h \
    sensor.h \
    wiredSensor.h \
    wirelessSensor.h \
    actuatorInstantiator.h \
    sensorInstantiator.h \
    sensorManager.h \
    RF24Interface.h \
    RF24Functions.h \
    wiringPiInterface.h

SOURCES += \
    actuator.cpp \
    sensor.cpp \
    wiredSensor.cpp \
    wirelessSensor.cpp \
    actuatorInstantiator.cpp \
    sensorInstantiator.cpp \
    sensorManagerService.cpp \
    sensorManager.cpp \
    RF24Interface.cpp \
    RF24Functions.cpp \
    wiringPiInterface.cpp

DISTFILES += \
    piHomeSensor.service

unix {
    target.path = /usr/share/pismarthome
    INSTALLS += target

    SYSTEMDSERVICE.files = piHomeSensor.service
    SYSTEMDSERVICE.path = /lib/systemd/system
    INSTALLS += SYSTEMDSERVICE
}
