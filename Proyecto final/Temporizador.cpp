#include "Temporizador.h"

Temporizador::Temporizador(float tiempoLimite)
    : tiempoActual(0.0f),
    tiempoLimite(tiempoLimite),
    activo(false) {
}

void Temporizador::iniciar() {
    activo = true;
}

void Temporizador::detener() {
    activo = false;
}

void Temporizador::reiniciar() {
    tiempoActual = 0.0f;
    activo = true;
}

void Temporizador::actualizar(float deltaTiempo) {
    if (activo) {
        tiempoActual += deltaTiempo;
    }
}

bool Temporizador::tiempoTerminado() const {
    return tiempoActual >= tiempoLimite;
}

float Temporizador::getTiempoActual() const {
    return tiempoActual;
}

float Temporizador::getTiempoRestante() const {
    return tiempoLimite - tiempoActual;
}