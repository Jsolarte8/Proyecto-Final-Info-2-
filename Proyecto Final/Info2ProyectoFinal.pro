QT += core gui widgets
CONFIG += c++17
TEMPLATE = app
TARGET = PertigaReinoMiniatura

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    IntroScreen.cpp \
    MenuScreen.cpp \
    CharacterSelectScreen.cpp \
    DifficultyScreen.cpp \
    GameScreen.cpp \
    FinalBattleScreen.cpp \
    Character.cpp \
    Personaje.cpp \
    Jugador.cpp \
    Enemigo.cpp \
    AraniaGuardiana.cpp \
    Obstaculo.cpp \
    Recolectable.cpp \
    Plataforma.cpp \
    Fisica.cpp \
    FisicaParabolica.cpp \
    FisicaElastica.cpp \
    FisicaFriccion.cpp \
    Temporizador.cpp \
    SistemaColisiones.cpp \
    Nivel.cpp \
    NivelUno.cpp \
    NivelDos.cpp \
    Juego.cpp

HEADERS += \
    MainWindow.h \
    IntroScreen.h \
    MenuScreen.h \
    CharacterSelectScreen.h \
    DifficultyScreen.h \
    GameScreen.h \
    FinalBattleScreen.h \
    Character.h \
    Entidad.h \
    Personaje.h \
    Jugador.h \
    Enemigo.h \
    AraniaGuardiana.h \
    Obstaculo.h \
    Recolectable.h \
    Plataforma.h \
    Fisica.h \
    FisicaParabolica.h \
    FisicaElastica.h \
    FisicaFriccion.h \
    Temporizador.h \
    SistemaColisiones.h \
    Nivel.h \
    NivelUno.h \
    NivelDos.h \
    Juego.h

RESOURCES += resources.qrc
