QT       += core dbus
QT       -= gui

TARGET = settingsManager
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    databaseSettingsManager.cpp \
    simplecrypt.cpp \
    settingsManagerService.cpp

HEADERS += databaseSettingsManager.h \
    simplecrypt.h
