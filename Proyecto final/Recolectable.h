#ifndef RECOLECTABLE_H
#define RECOLECTABLE_H

#include "Entidad.h"
#include "Jugador.h"
#include <string>

class Recolectable : public Entidad {
private:
    std::string tipo;
    int valor;
    bool recogido;

public:
    Recolectable(const std::string& tipo = "nectar",
                 float x = 0,
                 float y = 0,
                 float ancho = 25,
                 float alto = 25);

    virtual ~Recolectable();

    virtual void actualizar(float deltaTiempo) override;

    void recoger(Jugador* jugador);

    bool estaRecogido() const;
    std::string getTipo() const;
    int getValor() const;
};

#endif