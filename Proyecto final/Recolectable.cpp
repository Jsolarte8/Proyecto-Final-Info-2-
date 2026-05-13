#include "Recolectable.h"

Recolectable::Recolectable(const std::string& tipo, float x, float y, float ancho, float alto)
    : Entidad(x, y, ancho, alto),
    tipo(tipo),
    valor(10),
    recogido(false) {

    if (tipo == "nectar") {
        valor = 10;
    }
    else if (tipo == "semilla") {
        valor = 0;
    }
}

Recolectable::~Recolectable() {
}

void Recolectable::actualizar(float deltaTiempo) {
    (void)deltaTiempo;
}

void Recolectable::recoger(Jugador* jugador) {
    if (jugador == nullptr || recogido) {
        return;
    }

    if (tipo == "nectar") {
        jugador->aumentarPuntaje(valor);
        jugador->recolectarNectar();
    }
    else if (tipo == "semilla") {
        jugador->recolectarSemilla();
    }

    recogido = true;
    activo = false;
}

bool Recolectable::estaRecogido() const {
    return recogido;
}

std::string Recolectable::getTipo() const {
    return tipo;
}

int Recolectable::getValor() const {
    return valor;
}