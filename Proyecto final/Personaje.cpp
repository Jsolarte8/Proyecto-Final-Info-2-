#include "Personaje.h"

Personaje::Personaje(float x, float y, float ancho, float alto)
    : Entidad(x, y, ancho, alto),
    velocidadX(0),
    velocidadY(0),
    estado("quieto"),
    vidas(3) {
}

Personaje::~Personaje() {
}

void Personaje::actualizar(float deltaTiempo) {
    x += velocidadX * deltaTiempo;
    y += velocidadY * deltaTiempo;
}

void Personaje::mover(float dx, float dy) {
    x += dx;
    y += dy;
}

void Personaje::recibirDanio(int cantidad) {
    vidas -= cantidad;

    if (vidas <= 0) {
        vidas = 0;
        activo = false;
        estado = "derrotado";
    }
}

float Personaje::getVelocidadX() const {
    return velocidadX;
}

float Personaje::getVelocidadY() const {
    return velocidadY;
}

void Personaje::setVelocidadX(float vx) {
    velocidadX = vx;
}

void Personaje::setVelocidadY(float vy) {
    velocidadY = vy;
}

int Personaje::getVidas() const {
    return vidas;
}

void Personaje::setVidas(int nuevasVidas) {
    vidas = nuevasVidas;
}

std::string Personaje::getEstado() const {
    return estado;
}

void Personaje::setEstado(const std::string& nuevoEstado) {
    estado = nuevoEstado;
}