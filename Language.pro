TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    scaner.cpp \
    syntax.cpp \
    memuk.cpp \
    semant.cpp

HEADERS += \
    def.h \
    scaner.h \
    syntax.h \
    memuk.h \
    semant.h
