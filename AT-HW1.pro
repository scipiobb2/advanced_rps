TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXX = g++

SOURCES += main.cpp \
    advanced_rps_fight.cpp \
    coordinate.cpp \
    piece.cpp \
    move.cpp \
    player.cpp \
    board.cpp \
    game.cpp \
    error.cpp

HEADERS += \
    utils.hpp \
    advanced_rps_fight.h \
    coordinate.h \
    piece.h \
    move.h \
    player.h \
    board.h \
    game.h \
    error.h
