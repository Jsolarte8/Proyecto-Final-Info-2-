#include "FisicaFriccion.h"

#include <QtGlobal>

FisicaFriccion::FisicaFriccion(float coeficiente)
    : velocidadInicial(0.0f),
      coeficiente(coeficiente),
      velocidadResultado(0.0f)
{
}

void FisicaFriccion::aplicar(float velocidadInicial)
{
    this->velocidadInicial = velocidadInicial;
}

void FisicaFriccion::calcular(float deltaTiempo)
{
    Q_UNUSED(deltaTiempo);
    velocidadResultado = velocidadInicial * coeficiente;
}

float FisicaFriccion::getVelocidadResultado() const { return velocidadResultado; }
void FisicaFriccion::setCoeficiente(float value) { coeficiente = value; }
