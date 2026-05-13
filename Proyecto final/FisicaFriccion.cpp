#include "FisicaFriccion.h"

FisicaFriccion::FisicaFriccion(Jugador* jugador)
    : jugador(jugador),
    coeficienteFriccion(0.1f) {
}

FisicaFriccion::~FisicaFriccion() {
}

void FisicaFriccion::calcular(float deltaTiempo) {
    if (jugador == nullptr) {
        return;
    }

    float vx = jugador->getVelocidadX();
    float vy = jugador->getVelocidadY();

    vx -= vx * coeficienteFriccion * deltaTiempo;
    vy -= vy * coeficienteFriccion * deltaTiempo;

    jugador->setVelocidadX(vx);
    jugador->setVelocidadY(vy);
}

void FisicaFriccion::setCoeficienteFriccion(float coeficiente) {
    coeficienteFriccion = coeficiente;
}