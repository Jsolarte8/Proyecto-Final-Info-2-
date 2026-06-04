#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Jugador.h"
#include "Personaje.h"

class Enemigo : public Personaje
{
public:
    Enemigo(float x = 0.0f, float y = 0.0f, int vidas = 60);

    void perseguir(Jugador* jugador);
    void atacar(Jugador* jugador);

    float getAgresividad() const;
    float getRangoVision() const;
    int getDanio() const;

    void setAgresividad(float value);
    void setRangoVision(float value);
    void setDanio(int value);

protected:
    float agresividad;
    float rangoVision;
    int danio;
};

#endif
