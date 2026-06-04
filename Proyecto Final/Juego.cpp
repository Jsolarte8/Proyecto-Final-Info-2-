#include "Juego.h"
#include "NivelDos.h"
#include "NivelUno.h"

Juego::Juego()
    : nivelActual(nullptr),
      numeroNivel(1),
      puntajeTotal(0),
      vidas(100),
      dificultad(1),
      character(),
      finalizado(false)
{
}

Juego::~Juego()
{
    delete nivelActual;
    nivelActual = nullptr;
}

void Juego::setCharacter(const Character& character)
{
    this->character = character;
}

void Juego::setDifficulty(int difficulty)
{
    dificultad = difficulty;
}

void Juego::iniciar()
{
    delete nivelActual;
    numeroNivel = 1;
    puntajeTotal = 0;
    vidas = character.getHealth();
    finalizado = false;
    nivelActual = new NivelUno(dificultad, character);
    nivelActual->cargarNivel();
}

void Juego::actualizar(float deltaTiempo)
{
    if (!nivelActual || finalizado) {
        return;
    }

    nivelActual->actualizar(deltaTiempo);
    if (nivelActual->getJugador()) {
        puntajeTotal = nivelActual->getJugador()->getPuntaje();
        vidas = nivelActual->getJugador()->getVidas();
    }
}

void Juego::cambiarNivel()
{
    delete nivelActual;
    numeroNivel = 2;
    nivelActual = new NivelDos(dificultad, character);
    nivelActual->cargarNivel();
}

void Juego::reiniciarNivel()
{
    const int nivel = numeroNivel;
    delete nivelActual;
    nivelActual = nivel == 1 ? static_cast<Nivel*>(new NivelUno(dificultad, character))
                             : static_cast<Nivel*>(new NivelDos(dificultad, character));
    nivelActual->cargarNivel();
    finalizado = false;
}

void Juego::finalizarJuego()
{
    finalizado = true;
}

void Juego::moverJugador(float dx, float dy)
{
    if (nivelActual) {
        nivelActual->moverJugador(dx, dy);
    }
}

void Juego::saltarJugador()
{
    if (nivelActual) {
        nivelActual->saltarJugador();
    }
}

void Juego::cargarPertiga(float deltaTiempo)
{
    if (nivelActual) {
        nivelActual->cargarPertiga(deltaTiempo);
    }
}

void Juego::usarPertiga()
{
    if (nivelActual) {
        nivelActual->usarPertiga();
    }
}

void Juego::impulsoJugador()
{
    if (nivelActual) {
        nivelActual->impulsoJugador();
    }
}

Nivel* Juego::getNivelActual() const { return nivelActual; }
Jugador* Juego::getJugador() const { return nivelActual ? nivelActual->getJugador() : nullptr; }
int Juego::getNumeroNivel() const { return numeroNivel; }
int Juego::getPuntajeTotal() const { return puntajeTotal; }
int Juego::getVidas() const { return vidas; }
int Juego::getDificultad() const { return dificultad; }
