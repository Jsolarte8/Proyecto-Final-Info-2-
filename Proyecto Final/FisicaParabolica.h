#ifndef FISICAPARABOLICA_H
#define FISICAPARABOLICA_H

#include "Fisica.h"

class FisicaParabolica : public Fisica
{
public:
    FisicaParabolica(float gravedad = 1450.0f);

    void iniciar(float x, float y, float velocidadX, float velocidadY);
    void calcular(float deltaTiempo) override;

    float getX() const;
    float getY() const;
    float getVelocidadX() const;
    float getVelocidadY() const;
    void setGravedad(float value);

private:
    float x;
    float y;
    float velocidadX;
    float velocidadY;
    float gravedad;
};

#endif
