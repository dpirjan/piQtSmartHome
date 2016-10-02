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
LIBS += -lsystemd

SOURCES += utils.cpp \
    watchdogHelper.cpp \
    watchdogFunctions.cpp

HEADERS += utils.h \
    watchdogHelper.h \
    watchdogFunctions.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
