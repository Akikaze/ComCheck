QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = lib

CONFIG += c++11
CONFIG += plugin

QT += core
QT -= gui

INCLUDEPATH += ../../console

HEADERS += \
    template.hpp

SOURCES += \
    template.cpp

TARGET = plugin_Template

DESTDIR = ../../plugins

OTHER_FILES += \
    prefix.txt
