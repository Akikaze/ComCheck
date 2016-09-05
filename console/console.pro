QT += core
QT -= gui

CONFIG += c++11

TARGET = console
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    core.cpp

HEADERS += \
    core.hpp \
    iplugin.hpp \
    qt.hpp \
    std.hpp \
    structures.hpp
