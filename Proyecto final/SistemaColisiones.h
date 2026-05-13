#ifndef SISTEMACOLISIONES_H
#define SISTEMACOLISIONES_H

#include "Entidad.h"

class SistemaColisiones {
public:
    static bool verificarColision(Entidad* a, Entidad* b);
};

#endif