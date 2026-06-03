#ifndef SISTEMACOLISIONES_H
#define SISTEMACOLISIONES_H

#include "Entidad.h"
#include "Jugador.h"
#include "Plataforma.h"

class SistemaColisiones
{
public:
    static bool colisionan(const Entidad* a, const Entidad* b);
    static bool resolverJugadorPlataforma(Jugador* jugador, const Plataforma* plataforma);
};

#endif
