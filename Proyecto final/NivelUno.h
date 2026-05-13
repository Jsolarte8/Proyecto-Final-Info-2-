#ifndef NIVELUNO_H
#define NIVELUNO_H

#include "Nivel.h"
#include "Temporizador.h"
#include "FisicaParabolica.h"
#include "FisicaElastica.h"
#include "FisicaFriccion.h"

class NivelUno : public Nivel {
private:
    Temporizador* temporizador;
    FisicaParabolica* fisicaParabolica;
    FisicaElastica* fisicaElastica;
    FisicaFriccion* fisicaFriccion;

    float posicionMeta;

public:
    NivelUno();
    virtual ~NivelUno();

    virtual void cargarNivel() override;
    virtual void actualizar(float deltaTiempo) override;

    void validarMeta();
    bool tiempoTerminado() const;
};

#endif