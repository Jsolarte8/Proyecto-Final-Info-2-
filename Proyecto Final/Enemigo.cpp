#include "Enemigo.h"

#include <cmath>

Enemigo::Enemigo(float x, float y, int vidas)
    : Personaje(x, y, 56.0f, 56.0f, vidas),
      agresividad(1.0f),
      rangoVision(400.0f),
      danio(8)
{
}

void Enemigo::perseguir(Jugador* jugador)
{
    if (!jugador || !jugador->estaActivo()) {
        velocidadX = 0.0f;
        velocidadY = 0.0f;
        return;
    }

    const float dx = jugador->getX() - x;
    const float dy = jugador->getY() - y;
    const float distancia = std::sqrt(dx * dx + dy * dy);

    if (distancia > rangoVision || distancia < 1.0f) {
        velocidadX = 0.0f;
        velocidadY = 0.0f;
        return;
    }

    const float velocidad = 65.0f * agresividad;
    velocidadX = (dx / distancia) * velocidad;
    velocidadY = (dy / distancia) * velocidad;
    estado = "walk";
}

void Enemigo::atacar(Jugador* jugador)
{
    if (jugador && rect().intersects(jugador->rect())) {
        jugador->recibirDanio(danio);
    }
}

float Enemigo::getAgresividad() const { return agresividad; }
float Enemigo::getRangoVision() const { return rangoVision; }
int Enemigo::getDanio() const { return danio; }

void Enemigo::setAgresividad(float value) { agresividad = value; }
void Enemigo::setRangoVision(float value) { rangoVision = value; }
void Enemigo::setDanio(int value) { danio = value; }
