QT += qml quick quickcontrols2 dbus

TARGET = uiManager
CONFIG += c++11 debug

static {
    QT += svg
    QTPLUGIN += qtvirtualkeyboardplugin
}

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
    UIManager.h

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
