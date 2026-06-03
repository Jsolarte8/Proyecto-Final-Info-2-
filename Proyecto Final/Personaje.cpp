#include "Personaje.h"

#include <algorithm>

Personaje::Personaje(float x, float y, float ancho, float alto, int vidas)
    : Entidad(x, y, ancho, alto),
      velocidadX(0.0f),
      velocidadY(0.0f),
      estado("idle"),
      vidas(vidas)
{
}

void Personaje::actualizar(float deltaTiempo)
{
    x += velocidadX * deltaTiempo;
    y += velocidadY * deltaTiempo;
}

void Personaje::mover(float dx, float dy)
{
    velocidadX = dx;
    velocidadY = dy;
}

void Personaje::recibirDanio(int cantidad)
{
    if (cantidad <= 0 || !activo) {
        return;
    }

    vidas = std::max(0, vidas - cantidad);
    if (vidas == 0) {
        activo = false;
        estado = "derrotado";
    }
}

float Personaje::getVelocidadX() const { return velocidadX; }
float Personaje::getVelocidadY() const { return velocidadY; }
QString Personaje::getEstado() const { return estado; }
int Personaje::getVidas() const { return vidas; }

void Personaje::setVelocidadX(float value) { velocidadX = value; }
void Personaje::setVelocidadY(float value) { velocidadY = value; }
void Personaje::setEstado(const QString& value) { estado = value; }
void Personaje::setVidas(int value)
{
    vidas = std::max(0, value);
    activo = vidas > 0;
}
