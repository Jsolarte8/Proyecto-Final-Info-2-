#include "FisicaElastica.h"

#include <QtGlobal>
#include <algorithm>

FisicaElastica::FisicaElastica(float constante, float amortiguacion)
    : constante(constante),
      amortiguacion(amortiguacion),
      carga(0.0f),
      fuerzaResultante(0.0f)
{
}

void FisicaElastica::cargar(float energia)
{
    carga = std::clamp(energia, 0.0f, 100.0f);
}

float FisicaElastica::soltar()
{
    fuerzaResultante = carga * constante * amortiguacion;
    carga = 0.0f;
    return fuerzaResultante;
}

void FisicaElastica::calcular(float deltaTiempo)
{
    Q_UNUSED(deltaTiempo);
    fuerzaResultante = carga * constante * amortiguacion;
}

float FisicaElastica::getCarga() const { return carga; }
float FisicaElastica::getFuerzaResultante() const { return fuerzaResultante; }
