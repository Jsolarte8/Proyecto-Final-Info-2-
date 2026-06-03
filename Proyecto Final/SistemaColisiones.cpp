#include "SistemaColisiones.h"

bool SistemaColisiones::colisionan(const Entidad* a, const Entidad* b)
{
    return a && b && a->estaActivo() && b->estaActivo() && a->rect().intersects(b->rect());
}

bool SistemaColisiones::resolverJugadorPlataforma(Jugador* jugador, const Plataforma* plataforma)
{
    if (!jugador || !plataforma || !colisionan(jugador, plataforma)) {
        return false;
    }

    const float jugadorBase = jugador->getY() + jugador->getAlto();
    const float plataformaTecho = plataforma->getY();
    if (jugador->getVelocidadY() >= 0.0f && jugadorBase <= plataformaTecho + 28.0f) {
        jugador->aterrizar(plataformaTecho);
        return true;
    }

    return false;
}
