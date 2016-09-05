QT += core
QT -= gui

CONFIG += c++11

TARGET = TestPlugin
CONFIG += plugin

TEMPLATE = lib

INCLUDEPATH += ../../console

SOURCES += \
    test_plugin.cpp

HEADERS += \
    test_plugin.hpp

DESTDIR = ../../plugins
