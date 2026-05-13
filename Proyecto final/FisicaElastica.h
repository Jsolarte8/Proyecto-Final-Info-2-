#ifndef FISICAELASTICA_H
#define FISICAELASTICA_H

#include "Fisica.h"
#include "Jugador.h"

class FisicaElastica : public Fisica {
private:
    Jugador* jugador;
    float constanteElastica;
    float deformacion;
    float fuerza;

public:
    FisicaElastica(Jugador* jugador = nullptr);

    virtual ~FisicaElastica();

    virtual void calcular(float deltaTiempo) override;

    float getFuerza() const;
};

#endif