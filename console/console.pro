QMAKE_CXXFLAGS += -std=c++11

QT += core
QT -= gui

CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

TARGET = console

TEMPLATE = app

SOURCES += main.cpp \
    core.cpp \
    iui.cpp \
    console_ui.cpp \
    console_ui_commands.cpp \
    core_export.cpp

HEADERS += \
    core.hpp \
    iplugin.hpp \
    qt.hpp \
    std.hpp \
    structures.hpp \
    iui.hpp \
    console_ui.hpp

OTHER_FILES += \
    README.md \
    CC_style.css \
    prefix.txt
