#ifndef FISICAFRICCION_H
#define FISICAFRICCION_H

#include "Fisica.h"
#include "Jugador.h"

class FisicaFriccion : public Fisica {
private:
    Jugador* jugador;
    float coeficienteFriccion;

public:
    FisicaFriccion(Jugador* jugador = nullptr);

    virtual ~FisicaFriccion();

    virtual void calcular(float deltaTiempo) override;

    void setCoeficienteFriccion(float coeficiente);
};

#endif