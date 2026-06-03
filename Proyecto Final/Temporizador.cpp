#include "Temporizador.h"

#include <algorithm>

Temporizador::Temporizador(float segundos)
    : tiempoRestante(segundos),
      activo(false)
{
}

void Temporizador::iniciar(float segundos)
{
    tiempoRestante = segundos;
    activo = true;
}

void Temporizador::detener()
{
    activo = false;
}

void Temporizador::actualizar(float deltaTiempo)
{
    if (!activo) {
        return;
    }

    tiempoRestante = std::max(0.0f, tiempoRestante - deltaTiempo);
    if (tiempoRestante <= 0.0f) {
        activo = false;
    }
}

bool Temporizador::agotado() const { return tiempoRestante <= 0.0f; }
float Temporizador::getTiempoRestante() const { return tiempoRestante; }
bool Temporizador::estaActivo() const { return activo; }
