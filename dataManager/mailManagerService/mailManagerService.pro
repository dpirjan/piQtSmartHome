QT       += core dbus network
QT       -= gui

TARGET = mailManager
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += mailManagerService.cpp \
    mailManager.cpp

HEADERS += \
    mailManager.h
