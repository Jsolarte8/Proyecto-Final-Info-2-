#ifndef FISICAFRICCION_H
#define FISICAFRICCION_H

#include "Fisica.h"

class FisicaFriccion : public Fisica
{
public:
    FisicaFriccion(float coeficiente = 0.65f);

    void aplicar(float velocidadInicial);
    void calcular(float deltaTiempo) override;

    float getVelocidadResultado() const;
    void setCoeficiente(float value);

private:
    float velocidadInicial;
    float coeficiente;
    float velocidadResultado;
};

#endif
