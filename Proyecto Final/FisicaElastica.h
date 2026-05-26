#ifndef FISICAELASTICA_H
#define FISICAELASTICA_H

#include "Fisica.h"

class FisicaElastica : public Fisica
{
public:
    FisicaElastica(float constante = 7.5f, float amortiguacion = 0.82f);

    void cargar(float energia);
    float soltar();
    void calcular(float deltaTiempo) override;

    float getCarga() const;
    float getFuerzaResultante() const;

private:
    float constante;
    float amortiguacion;
    float carga;
    float fuerzaResultante;
};

#endif
