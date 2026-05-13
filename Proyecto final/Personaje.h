#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "Entidad.h"
#include <string>

class Personaje : public Entidad {
protected:
    float velocidadX;
    float velocidadY;
    std::string estado;
    int vidas;

public:
    Personaje(float x = 0, float y = 0, float ancho = 0, float alto = 0);

    virtual ~Personaje();

    virtual void actualizar(float deltaTiempo) override;

    virtual void mover(float dx, float dy);
    virtual void recibirDanio(int cantidad);

    float getVelocidadX() const;
    float getVelocidadY() const;

    void setVelocidadX(float vx);
    void setVelocidadY(float vy);

    int getVidas() const;
    void setVidas(int nuevasVidas);

    std::string getEstado() const;
    void setEstado(const std::string& nuevoEstado);
};

#endif