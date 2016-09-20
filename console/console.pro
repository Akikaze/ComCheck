QMAKE_CXXFLAGS += -std=c++11

QT += core
QT -= gui

CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

TARGET = console

TEMPLATE = app

SOURCES += main.cpp \
    core.cpp

HEADERS += \
    core.hpp \
    iplugin.hpp \
    qt.hpp \
    std.hpp \
    structures.hpp
