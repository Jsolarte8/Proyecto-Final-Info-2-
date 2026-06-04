#include "Nivel.h"

#include <QtGlobal>

Nivel::Nivel(int dificultad, const Character& character)
    : jugador(nullptr),
      dificultad(dificultad),
      character(character),
      estado(EstadoNivel::EnCurso)
{
}

Nivel::~Nivel()
{
    limpiarEntidades();
}

void Nivel::actualizar(float deltaTiempo)
{
    if (jugador) {
        jugador->actualizar(deltaTiempo);
    }
}

void Nivel::verificarColisiones()
{
}

void Nivel::moverJugador(float dx, float dy)
{
    Q_UNUSED(dx);
    Q_UNUSED(dy);
}

void Nivel::saltarJugador()
{
}

void Nivel::cargarPertiga(float deltaTiempo)
{
    Q_UNUSED(deltaTiempo);
}

void Nivel::usarPertiga()
{
}

void Nivel::impulsoJugador()
{
}

Jugador* Nivel::getJugador() const { return jugador; }
const std::vector<Obstaculo*>& Nivel::getObstaculos() const { return obstaculos; }
const std::vector<Recolectable*>& Nivel::getRecolectables() const { return recolectables; }
const std::vector<Plataforma*>& Nivel::getPlataformas() const { return plataformas; }
EstadoNivel Nivel::getEstado() const { return estado; }
int Nivel::getDificultad() const { return dificultad; }

void Nivel::limpiarEntidades()
{
    delete jugador;
    jugador = nullptr;

    for (Obstaculo* obstaculo : obstaculos) {
        delete obstaculo;
    }
    obstaculos.clear();

    for (Recolectable* recolectable : recolectables) {
        delete recolectable;
    }
    recolectables.clear();

    for (Plataforma* plataforma : plataformas) {
        delete plataforma;
    }
    plataformas.clear();
}
