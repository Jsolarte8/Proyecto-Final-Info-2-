#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include "Entidad.h"

#include <QString>

class Plataforma : public Entidad
{
public:
    Plataforma(float x = 0.0f,
               float y = 0.0f,
               float ancho = 140.0f,
               float alto = 24.0f,
               const QString& tipo = "hoja");

    QString getTipo() const;

private:
    QString tipo;
};

#endif
