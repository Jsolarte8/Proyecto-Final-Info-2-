#include "Enemigo.h"
#include <cmath>

Enemigo::Enemigo(float x, float y)
    : Personaje(x, y, 50, 50),
    agresividad(1.0f),
    rangoVision(150.0f),
    danio(1) {
    estado = "patrullando";
}

Enemigo::~Enemigo() {
}

void Enemigo::perseguir(Jugador* jugador) {
    if (jugador == nullptr) {
        return;
    }

    if (jugador->getX() > x) {
        velocidadX = 50 * agresividad;
    } else {
        velocidadX = -50 * agresividad;
    }

    if (jugador->getY() > y) {
        velocidadY = 50 * agresividad;
    } else {
        velocidadY = -50 * agresividad;
    }

    estado = "persiguiendo";
}

void Enemigo::atacar(Jugador* jugador) {
    if (jugador != nullptr) {
        jugador->recibirDanio(danio);
        estado = "atacando";
    }
}

float Enemigo::calcularDistancia(Jugador* jugador) const {
    if (jugador == nullptr) {
        return 999999.0f;
    }

    float dx = jugador->getX() - x;
    float dy = jugador->getY() - y;

    return std::sqrt(dx * dx + dy * dy);
}

float Enemigo::getAgresividad() const {
    return agresividad;
}

void Enemigo::setAgresividad(float nuevaAgresividad) {
    agresividad = nuevaAgresividad;
}

float Enemigo::getRangoVision() const {
    return rangoVision;
}

void Enemigo::setRangoVision(float nuevoRango) {
    rangoVision = nuevoRango;
}

int Enemigo::getDanio() const {
    return danio;
}

void Enemigo::setDanio(int nuevoDanio) {
    danio = nuevoDanio;
}