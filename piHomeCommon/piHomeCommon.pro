QT -= gui
QT += dbus

TARGET = piHomeCommon
TEMPLATE = lib
CONFIG += debug

CONFIG(debug, debug|release) {
    message("$${TARGET} - debug mode")
}else {
    message("$${TARGET} - release mode")
}

DEFINES += QT_MESSAGELOGCONTEXT
LIBS += -lsystemd

SOURCES += homeAlarmInfo.cpp \
           smartHomeInfo.cpp \
           piHomeCommon.cpp \
           watchdogHelper.cpp \
           watchdogFunctions.cpp \
           mailManagerInterface.cpp \
           databaseManagerInterface.cpp \
           io.cpp \
           userManagerInterface.cpp \
           alarmManagerInterface.cpp

HEADERS += common.h \
           homeAlarmInfo.h \
           smartHomeInfo.h \
           piHomeCommon.h \
           watchdogHelper.h \
           watchdogFunctions.h \
           mailManagerInterface.h \
           databaseManagerInterface.h \
           io.h \
           userManagerInterface.h \
           alarmManagerInterface.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
