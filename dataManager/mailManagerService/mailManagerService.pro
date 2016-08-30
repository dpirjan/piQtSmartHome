QT       += core dbus network
QT       -= gui

TARGET = mailManager
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

INCLUDEPATH += ../../utils
LIBS += -L../../utils -lpiHomeUtils

SOURCES += mailManagerService.cpp \
    mailManager.cpp

HEADERS += \
    mailManager.h

unix {
    target.path = /usr/share/pismarthome
    INSTALLS += target
}
