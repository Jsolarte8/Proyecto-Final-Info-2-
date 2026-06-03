#include "Plataforma.h"

Plataforma::Plataforma(float x, float y, float ancho, float alto, const QString& tipo)
    : Entidad(x, y, ancho, alto),
      tipo(tipo)
{
}

QString Plataforma::getTipo() const
{
    return tipo;
}
