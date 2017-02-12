QT += core dbus network
QT -= gui

TARGET = webManager
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

HEADERS += listdatacontroller.h \
        logincontroller.h \
        requestmapper.h \
        webManager.h \
    alarmcontroller.h \
    lockcontroller.h

SOURCES += listdatacontroller.cpp \
        logincontroller.cpp \
        requestmapper.cpp \
        webManagerService.cpp \
        webManager.cpp \
    alarmcontroller.cpp \
    lockcontroller.cpp

DISTFILES += piHomeWeb.service

unix {
    target.path = /usr/share/pismarthome
    INSTALLS += target

    SYSTEMDSERVICE.files = piHomeWeb.service
    SYSTEMDSERVICE.path = /lib/systemd/system
    INSTALLS += SYSTEMDSERVICE
}

include(QtWebApp/httpserver/httpserver.pri)
##include(QtWebApp/templateengine/templateengine.pri)
