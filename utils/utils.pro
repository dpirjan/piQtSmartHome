QT -= gui

TARGET = piHomeUtils
TEMPLATE = lib
CONFIG += debug

CONFIG(debug, debug|release) {
    message("$${TARGET} - debug mode")
}else {
    message("$${TARGET} - release mode")
}

DEFINES += QT_MESSAGELOGCONTEXT

SOURCES += utils.cpp

HEADERS += utils.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
