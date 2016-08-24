QT += dbus

TEMPLATE = lib
SOURCES = homeAlarmInfo.cpp \
    smartHomeInfo.cpp
HEADERS = homeAlarmInfo.h \
    smartHomeInfo.h

target.path = /usr/lib
INSTALLS += target
