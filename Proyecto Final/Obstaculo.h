#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "Entidad.h"
#include "Jugador.h"

#include <QString>

class Obstaculo : public Entidad
{
public:
    Obstaculo(const QString& tipo = "piedra",
              float x = 0.0f,
              float y = 0.0f,
              float ancho = 48.0f,
              float alto = 48.0f,
              int danio = 10,
              float efectoVelocidad = 1.0f,
              bool movil = false);

    void actualizar(float deltaTiempo) override;
    void afectar(Jugador* jugador);

    QString getTipo() const;
    int getDanio() const;
    float getEfectoVelocidad() const;
    bool esMovil() const;

private:
    QString tipo;
    int danio;
    float efectoVelocidad;
    bool movil;
    float yBase;
    float direccion;
};

#endif
