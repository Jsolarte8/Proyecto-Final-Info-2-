#include "Obstaculo.h"

Obstaculo::Obstaculo(const std::string& tipo, float x, float y, float ancho, float alto)
    : Entidad(x, y, ancho, alto),
    tipo(tipo),
    danio(0),
    efectoVelocidad(1.0f) {

    if (tipo == "piedra" || tipo == "raiz") {
        danio = 1;
        efectoVelocidad = 0.5f;
    }
    else if (tipo == "charco" || tipo == "barro") {
        danio = 0;
        efectoVelocidad = 0.4f;
    }
    else if (tipo == "hilo_pegajoso") {
        danio = 0;
        efectoVelocidad = 0.25f;
    }
}

Obstaculo::~Obstaculo() {
}

void Obstaculo::actualizar(float deltaTiempo) {
    (void)deltaTiempo;
}

void Obstaculo::afectar(Jugador* jugador) {
    if (jugador == nullptr) {
        return;
    }

    if (danio > 0) {
        jugador->recibirDanio(danio);
    }

    jugador->setVelocidadX(jugador->getVelocidadX() * efectoVelocidad);
    jugador->setVelocidadY(jugador->getVelocidadY() * efectoVelocidad);
}

std::string Obstaculo::getTipo() const {
    return tipo;
}

int Obstaculo::getDanio() const {
    return danio;
}

float Obstaculo::getEfectoVelocidad() const {
    return efectoVelocidad;
}