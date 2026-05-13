TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        AraniaGuardiana.cpp \
        Enemigo.cpp \
        FisicaElastica.cpp \
        FisicaFriccion.cpp \
        FisicaParabolica.cpp \
        Gestorguardado.cpp \
        Juego.cpp \
        Jugador.cpp \
        Nivel.cpp \
        NivelDos.cpp \
        NivelUno.cpp \
        Obstaculo.cpp \
        Personaje.cpp \
        Plataforma.cpp \
        Recolectable.cpp \
        SistemaColisiones.cpp \
        Temporizador.cpp \
        main.cpp

HEADERS += \
    AraniaGuardiana.h \
    Enemigo.h \
    Entidad.h \
    Fisica.h \
    FisicaElastica.h \
    FisicaFriccion.h \
    FisicaParabolica.h \
    Gestorguardado.h \
    Juego.h \
    Jugador.h \
    Nivel.h \
    NivelDos.h \
    NivelUno.h \
    Obstaculo.h \
    Personaje.h \
    Plataforma.h \
    Recolectable.h \
    SistemaColisiones.h \
    Temporizador.h
