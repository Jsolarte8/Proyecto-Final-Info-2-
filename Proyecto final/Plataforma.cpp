#include "Plataforma.h"

Plataforma::Plataforma(const std::string& tipo, float x, float y, float ancho, float alto)
    : Entidad(x, y, ancho, alto),
    tipo(tipo),
    resistencia(1.0f),
    estable(true) {

    if (tipo == "hoja") {
        resistencia = 0.8f;
        estable = true;
    }
    else if (tipo == "rama") {
        resistencia = 1.0f;
        estable = true;
    }
    else if (tipo == "telarana") {
        resistencia = 0.5f;
        estable = false;
    }
}

Plataforma::~Plataforma() {
}

void Plataforma::actualizar(float deltaTiempo) {
    (void)deltaTiempo;
}

void Plataforma::soportarJugador(Jugador* jugador) {
    if (jugador == nullptr) {
        return;
    }

    jugador->aterrizar();
}

std::string Plataforma::getTipo() const {
    return tipo;
}

float Plataforma::getResistencia() const {
    return resistencia;
}

bool Plataforma::esEstable() const {
    return estable;
}