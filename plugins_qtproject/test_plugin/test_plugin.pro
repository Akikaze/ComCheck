QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = lib

CONFIG += c++11
CONFIG += plugin

QT += core
QT -= gui

INCLUDEPATH += ../../console

HEADERS += \
    test_plugin.hpp

SOURCES += \
    test_plugin.cpp

TARGET = plugin_TestPlugin

DESTDIR = ../../plugins
