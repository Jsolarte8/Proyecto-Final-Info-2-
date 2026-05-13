#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "Entidad.h"
#include "Jugador.h"
#include <string>

class Obstaculo : public Entidad {
private:
    std::string tipo;
    int danio;
    float efectoVelocidad;

public:
    Obstaculo(const std::string& tipo = "piedra",
              float x = 0,
              float y = 0,
              float ancho = 40,
              float alto = 40);

    virtual ~Obstaculo();

    virtual void actualizar(float deltaTiempo) override;

    void afectar(Jugador* jugador);

    std::string getTipo() const;
    int getDanio() const;
    float getEfectoVelocidad() const;
};

#endif