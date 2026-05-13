#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include "Entidad.h"
#include "Jugador.h"
#include <string>

class Plataforma : public Entidad {
private:
    std::string tipo;
    float resistencia;
    bool estable;

public:
    Plataforma(const std::string& tipo = "hoja",
               float x = 0,
               float y = 0,
               float ancho = 80,
               float alto = 20);

    virtual ~Plataforma();

    virtual void actualizar(float deltaTiempo) override;

    void soportarJugador(Jugador* jugador);

    std::string getTipo() const;
    float getResistencia() const;
    bool esEstable() const;
};

#endif