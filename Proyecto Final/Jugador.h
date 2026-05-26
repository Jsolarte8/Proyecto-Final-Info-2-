#ifndef JUGADOR_H
#define JUGADOR_H

#include "Personaje.h"

class Jugador : public Personaje
{
public:
    Jugador(float x = 0.0f, float y = 0.0f, int vidas = 100);

    int getPuntaje() const;
    int getNectar() const;
    int getSemillas() const;
    float getEnergiaPertiga() const;
    bool estaEnAire() const;

    void setPuntaje(int value);
    void setNectar(int value);
    void setSemillas(int value);
    void setEnergiaPertiga(float value);
    void setEnAire(bool value);

    void cargarPertiga(float deltaTiempo);
    float usarPertiga();
    void aterrizar(float sueloY);
    void recolectarNectar(int valor = 10);
    void recolectarSemilla(int valor = 50);

private:
    int puntaje;
    int nectar;
    int semillas;
    float energiaPertiga;
    bool enAire;
};

#endif
