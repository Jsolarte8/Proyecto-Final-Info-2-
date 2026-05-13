#ifndef JUEGO_H
#define JUEGO_H

#include "Nivel.h"
#include "NivelUno.h"
#include "NivelDos.h"
#include "GestorGuardado.h"

class Juego {
private:
    Nivel* nivelActual;
    GestorGuardado gestorGuardado;

    int numeroNivel;
    int puntajeTotal;
    int vidas;
    bool juegoActivo;
    bool juegoCompletado;

public:
    Juego();
    ~Juego();

    void iniciar();
    void actualizar(float deltaTiempo);
    void cambiarNivel();
    void reiniciarNivel();
    void finalizarJuego();

    Nivel* getNivelActual() const;

    int getNumeroNivel() const;
    int getPuntajeTotal() const;
    int getVidas() const;

    bool estaActivo() const;
    bool estaCompletado() const;
};

#endif