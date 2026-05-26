TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Fisica.cpp \
        FisicaElastica.cpp \
        FisicaFriccion.cpp \
        FisicaParabolica.cpp \
        Jugador.cpp \
        main.cpp

HEADERS += \
    Fisica.h \
    FisicaElastica.h \
    FisicaFriccion.h \
    FisicaParabolica.h \
    Jugador.h
