QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = lib

CONFIG += c++11
CONFIG += plugin

QT += core
QT -= gui

INCLUDEPATH += ../../console

HEADERS += \
    cpp.hpp

SOURCES += \
    cpp.cpp

TARGET = plugin_CPP

DESTDIR = ../../plugins

OTHER_FILES += \
    prefix.txt
