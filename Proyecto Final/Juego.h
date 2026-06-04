#ifndef JUEGO_H
#define JUEGO_H

#include "Character.h"
#include "Nivel.h"

class Juego
{
public:
    Juego();
    ~Juego();

    void setCharacter(const Character& character);
    void setDifficulty(int difficulty);

    void iniciar();
    void actualizar(float deltaTiempo);
    void cambiarNivel();
    void reiniciarNivel();
    void finalizarJuego();

    void moverJugador(float dx, float dy);
    void saltarJugador();
    void cargarPertiga(float deltaTiempo);
    void usarPertiga();
    void impulsoJugador();

    Nivel* getNivelActual() const;
    Jugador* getJugador() const;
    int getNumeroNivel() const;
    int getPuntajeTotal() const;
    int getVidas() const;
    int getDificultad() const;

private:
    Nivel* nivelActual;
    int numeroNivel;
    int puntajeTotal;
    int vidas;
    int dificultad;
    Character character;
    bool finalizado;
};

#endif
