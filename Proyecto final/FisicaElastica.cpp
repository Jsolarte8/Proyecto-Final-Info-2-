#include "FisicaElastica.h"

FisicaElastica::FisicaElastica(Jugador* jugador)
    : jugador(jugador),
    constanteElastica(2.0f),
    deformacion(0.0f),
    fuerza(0.0f) {
}

FisicaElastica::~FisicaElastica() {
}

void FisicaElastica::calcular(float deltaTiempo) {
    (void)deltaTiempo;

    if (jugador == nullptr) {
        return;
    }

    deformacion = jugador->getEnergiaPertiga();
    fuerza = constanteElastica * deformacion;
}

float FisicaElastica::getFuerza() const {
    return fuerza;
}