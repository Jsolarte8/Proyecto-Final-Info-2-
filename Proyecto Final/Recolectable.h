#ifndef RECOLECTABLE_H
#define RECOLECTABLE_H

#include "Entidad.h"
#include "Jugador.h"

#include <QString>

class Recolectable : public Entidad
{
public:
    Recolectable(const QString& tipo = "nectar",
                 float x = 0.0f,
                 float y = 0.0f,
                 float ancho = 34.0f,
                 float alto = 34.0f,
                 int valor = 10);

    void recoger(Jugador* jugador);

    QString getTipo() const;
    int getValor() const;
    bool estaRecogido() const;

private:
    QString tipo;
    int valor;
    bool recogido;
};

#endif
