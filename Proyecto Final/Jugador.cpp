#include "Jugador.h"

#include <algorithm>

Jugador::Jugador(float x, float y, int vidas)
    : Personaje(x, y, 48.0f, 64.0f, vidas),
      puntaje(0),
      nectar(0),
      semillas(0),
      energiaPertiga(0.0f),
      enAire(false)
{
}

int Jugador::getPuntaje() const { return puntaje; }
int Jugador::getNectar() const { return nectar; }
int Jugador::getSemillas() const { return semillas; }
float Jugador::getEnergiaPertiga() const { return energiaPertiga; }
bool Jugador::estaEnAire() const { return enAire; }

void Jugador::setPuntaje(int value) { puntaje = value; }
void Jugador::setNectar(int value) { nectar = value; }
void Jugador::setSemillas(int value) { semillas = value; }
void Jugador::setEnergiaPertiga(float value)
{
    energiaPertiga = std::clamp(value, 0.0f, 100.0f);
}

void Jugador::setEnAire(bool value) { enAire = value; }

void Jugador::cargarPertiga(float deltaTiempo)
{
    energiaPertiga = std::clamp(energiaPertiga + deltaTiempo * 55.0f, 0.0f, 100.0f);
    estado = "charge";
}

float Jugador::usarPertiga()
{
    const float energia = energiaPertiga;
    energiaPertiga = 0.0f;
    estado = "jump";
    enAire = true;
    return energia;
}

void Jugador::aterrizar(float sueloY)
{
    y = sueloY - alto;
    velocidadY = 0.0f;
    enAire = false;
    if (estado == "jump") {
        estado = "idle";
    }
}

void Jugador::recolectarNectar(int valor)
{
    ++nectar;
    puntaje += valor;
}

void Jugador::recolectarSemilla(int valor)
{
    ++semillas;
    puntaje += valor;
}
