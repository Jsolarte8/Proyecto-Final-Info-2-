#include "Juego.h"

Juego::Juego()
    : nivelActual(nullptr),
    numeroNivel(1),
    puntajeTotal(0),
    vidas(3),
    juegoActivo(false),
    juegoCompletado(false) {
}

Juego::~Juego() {
    delete nivelActual;
}

void Juego::iniciar() {
    delete nivelActual;

    numeroNivel = 1;
    puntajeTotal = 0;
    vidas = 3;
    juegoActivo = true;
    juegoCompletado = false;

    nivelActual = new NivelUno();
    nivelActual->cargarNivel();
}

void Juego::actualizar(float deltaTiempo) {
    if (!juegoActivo || nivelActual == nullptr) {
        return;
    }

    nivelActual->actualizar(deltaTiempo);

    Jugador* jugador = nivelActual->getJugador();

    if (jugador != nullptr) {
        puntajeTotal = jugador->getPuntaje();
        vidas = jugador->getVidas();

        if (vidas <= 0) {
            finalizarJuego();
            return;
        }
    }

    if (nivelActual->estaCompletado()) {
        cambiarNivel();
    }
}

void Juego::cambiarNivel() {
    delete nivelActual;
    nivelActual = nullptr;

    if (numeroNivel == 1) {
        numeroNivel = 2;
        nivelActual = new NivelDos();
        nivelActual->cargarNivel();
    }
    else {
        juegoCompletado = true;
        finalizarJuego();
    }
}

void Juego::reiniciarNivel() {
    delete nivelActual;
    nivelActual = nullptr;

    if (numeroNivel == 1) {
        nivelActual = new NivelUno();
    }
    else {
        nivelActual = new NivelDos();
    }

    nivelActual->cargarNivel();
}

void Juego::finalizarJuego() {
    juegoActivo = false;

    gestorGuardado.guardarProgreso(
        "progreso.txt",
        numeroNivel,
        puntajeTotal,
        0
        );
}

Nivel* Juego::getNivelActual() const {
    return nivelActual;
}

int Juego::getNumeroNivel() const {
    return numeroNivel;
}

int Juego::getPuntajeTotal() const {
    return puntajeTotal;
}

int Juego::getVidas() const {
    return vidas;
}

bool Juego::estaActivo() const {
    return juegoActivo;
}

bool Juego::estaCompletado() const {
    return juegoCompletado;
}