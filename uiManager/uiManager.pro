QT += qml quick quickcontrols2 dbus

TARGET = uiManager
CONFIG += c++11 debug

TEMPLATE = app

CONFIG(debug, debug|release) {
    message("$${TARGET} - debug mode")
    DEFINES += QT_MESSAGELOGCONTEXT
} else {
    message("$${TARGET} - release mode")
}

INCLUDEPATH += ../piHomeCommon
LIBS += -L../piHomeCommon -lpiHomeCommon

SOURCES += \
    uiManagerService.cpp \
    UIManager.cpp

RESOURCES += qml.qrc

DISTFILES += \
    piHomeUI.service

unix {
    target.path = /usr/share/pismarthome
    INSTALLS += target

    SYSTEMDSERVICE.files = piHomeUI.service
    SYSTEMDSERVICE.path = /lib/systemd/system
    INSTALLS += SYSTEMDSERVICE
}

HEADERS += \
    UIManager.h
