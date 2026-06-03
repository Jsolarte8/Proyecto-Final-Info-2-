#include "Obstaculo.h"

#include <algorithm>

Obstaculo::Obstaculo(const QString& tipo,
                     float x,
                     float y,
                     float ancho,
                     float alto,
                     int danio,
                     float efectoVelocidad,
                     bool movil)
    : Entidad(x, y, ancho, alto),
      tipo(tipo),
      danio(danio),
      efectoVelocidad(efectoVelocidad),
      movil(movil),
      yBase(y),
      direccion(1.0f)
{
}

void Obstaculo::actualizar(float deltaTiempo)
{
    if (!movil || !activo) {
        return;
    }

    y += direccion * 95.0f * deltaTiempo;
    if (y > yBase + 90.0f) {
        y = yBase + 90.0f;
        direccion = -1.0f;
    } else if (y < yBase - 60.0f) {
        y = yBase - 60.0f;
        direccion = 1.0f;
    }
}

void Obstaculo::afectar(Jugador* jugador)
{
    if (!jugador || !activo || !rect().intersects(jugador->rect())) {
        return;
    }

    if (danio > 0) {
        jugador->recibirDanio(danio);
        if (tipo == "gota") {
            activo = false;
        }
    }
}

QString Obstaculo::getTipo() const { return tipo; }
int Obstaculo::getDanio() const { return danio; }
float Obstaculo::getEfectoVelocidad() const { return efectoVelocidad; }
bool Obstaculo::esMovil() const { return movil; }
