#include "FisicaParabolica.h"
#include <cmath>

FisicaParabolica::FisicaParabolica(Jugador* jugador)
    : jugador(jugador),
    velocidadInicial(150.0f),
    angulo(45.0f),
    gravedad(9.8f),
    tiempo(0.0f) {
}

FisicaParabolica::~FisicaParabolica() {
}

void FisicaParabolica::calcular(float deltaTiempo) {
    if (jugador == nullptr || !jugador->estaEnAire()) {
        return;
    }

    tiempo += deltaTiempo;

    float radianes = angulo * 3.14159265f / 180.0f;

    float vx = velocidadInicial * std::cos(radianes);
    float vy = velocidadInicial * std::sin(radianes) - gravedad * tiempo;

    jugador->setVelocidadX(vx);
    jugador->setVelocidadY(-vy);
}

void FisicaParabolica::configurar(float nuevaVelocidadInicial,
                                  float nuevoAngulo,
                                  float nuevaGravedad) {
    velocidadInicial = nuevaVelocidadInicial;
    angulo = nuevoAngulo;
    gravedad = nuevaGravedad;
}

void FisicaParabolica::reiniciarTiempo() {
    tiempo = 0.0f;
}