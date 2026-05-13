#ifndef NIVELDOS_H
#define NIVELDOS_H

#include "Nivel.h"
#include "AraniaGuardiana.h"
#include "FisicaParabolica.h"
#include "FisicaElastica.h"
#include "FisicaFriccion.h"

class NivelDos : public Nivel {
private:
    AraniaGuardiana* arania;
    FisicaParabolica* fisicaParabolica;
    FisicaElastica* fisicaElastica;
    FisicaFriccion* fisicaFriccion;

    bool salidaActiva;
    float salidaX;
    float salidaY;

public:
    NivelDos();
    virtual ~NivelDos();

    virtual void cargarNivel() override;
    virtual void actualizar(float deltaTiempo) override;
    virtual void verificarColisiones() override;

    void validarSalida();
    AraniaGuardiana* getArania() const;
};

#endif