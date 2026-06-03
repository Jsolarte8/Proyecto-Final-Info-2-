#include "Recolectable.h"

Recolectable::Recolectable(const QString& tipo,
                           float x,
                           float y,
                           float ancho,
                           float alto,
                           int valor)
    : Entidad(x, y, ancho, alto),
      tipo(tipo),
      valor(valor),
      recogido(false)
{
}

void Recolectable::recoger(Jugador* jugador)
{
    if (!jugador || recogido || !activo || !rect().intersects(jugador->rect())) {
        return;
    }

    if (tipo == "semilla") {
        jugador->recolectarSemilla(valor);
    } else {
        jugador->recolectarNectar(valor);
    }

    recogido = true;
    activo = false;
}

QString Recolectable::getTipo() const { return tipo; }
int Recolectable::getValor() const { return valor; }
bool Recolectable::estaRecogido() const { return recogido; }
