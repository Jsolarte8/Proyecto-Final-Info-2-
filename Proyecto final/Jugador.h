#ifndef JUGADOR_H
#define JUGADOR_H

#include "Personaje.h"

class Jugador : public Personaje {
private:
    int puntaje;
    int nectar;
    int semillas;
    float energiaPertiga;
    bool enAire;

public:
    Jugador(float x = 0, float y = 0);

    virtual ~Jugador();

    virtual void actualizar(float deltaTiempo) override;

    void cargarPertiga(float cantidad);
    void usarPertiga();
    void caer();
    void aterrizar();

    void aumentarPuntaje(int valor);
    void recolectarNectar();
    void recolectarSemilla();

    int getPuntaje() const;
    int getNectar() const;
    int getSemillas() const;

    float getEnergiaPertiga() const;
    void setEnergiaPertiga(float energia);

    bool estaEnAire() const;
};

#endif