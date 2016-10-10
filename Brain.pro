TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    interpreter.cpp \
    itoa.cpp

HEADERS += \
    interpreter.h \
    utils.h \
    tokens.h


unix|win32: LIBS += -lncurses
