#ifndef FISICAPARABOLICA_H
#define FISICAPARABOLICA_H

#include "Fisica.h"
#include "Jugador.h"

class FisicaParabolica : public Fisica {
private:
    Jugador* jugador;
    float velocidadInicial;
    float angulo;
    float gravedad;
    float tiempo;

public:
    FisicaParabolica(Jugador* jugador = nullptr);

    virtual ~FisicaParabolica();

    virtual void calcular(float deltaTiempo) override;

    void configurar(float velocidadInicial, float angulo, float gravedad);
    void reiniciarTiempo();
};

#endif