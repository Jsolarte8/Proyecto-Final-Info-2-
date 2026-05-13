#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Personaje.h"
#include "Jugador.h"

class Enemigo : public Personaje {
protected:
    float agresividad;
    float rangoVision;
    int danio;

public:
    Enemigo(float x = 0, float y = 0);

    virtual ~Enemigo();

    virtual void perseguir(Jugador* jugador);
    virtual void atacar(Jugador* jugador);

    float calcularDistancia(Jugador* jugador) const;

    float getAgresividad() const;
    void setAgresividad(float nuevaAgresividad);

    float getRangoVision() const;
    void setRangoVision(float nuevoRango);

    int getDanio() const;
    void setDanio(int nuevoDanio);
};

#endif