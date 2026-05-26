#include "FisicaParabolica.h"

FisicaParabolica::FisicaParabolica(float gravedad)
    : x(0.0f),
      y(0.0f),
      velocidadX(0.0f),
      velocidadY(0.0f),
      gravedad(gravedad)
{
}

void FisicaParabolica::iniciar(float x, float y, float velocidadX, float velocidadY)
{
    this->x = x;
    this->y = y;
    this->velocidadX = velocidadX;
    this->velocidadY = velocidadY;
}

void FisicaParabolica::calcular(float deltaTiempo)
{
    velocidadY += gravedad * deltaTiempo;
    x += velocidadX * deltaTiempo;
    y += velocidadY * deltaTiempo;
}

float FisicaParabolica::getX() const { return x; }
float FisicaParabolica::getY() const { return y; }
float FisicaParabolica::getVelocidadX() const { return velocidadX; }
float FisicaParabolica::getVelocidadY() const { return velocidadY; }
void FisicaParabolica::setGravedad(float value) { gravedad = value; }
