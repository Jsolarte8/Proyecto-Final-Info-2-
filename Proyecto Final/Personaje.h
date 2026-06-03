#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "Entidad.h"

#include <QString>

class Personaje : public Entidad
{
public:
    Personaje(float x = 0.0f, float y = 0.0f, float ancho = 48.0f, float alto = 64.0f, int vidas = 100);
    ~Personaje() override = default;

    void actualizar(float deltaTiempo) override;
    void mover(float dx, float dy);
    void recibirDanio(int cantidad);

    float getVelocidadX() const;
    float getVelocidadY() const;
    QString getEstado() const;
    int getVidas() const;

    void setVelocidadX(float value);
    void setVelocidadY(float value);
    void setEstado(const QString& value);
    void setVidas(int value);

protected:
    float velocidadX;
    float velocidadY;
    QString estado;
    int vidas;
};

#endif
