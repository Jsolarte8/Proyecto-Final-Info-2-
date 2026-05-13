/*
#include <iostream>
#include "Juego.h"

int main()
{
    Juego juego;

    juego.iniciar();

    for (int i = 0; i < 10; i++) {
        juego.actualizar(0.016f);
    }

    std::cout << "Juego iniciado correctamente" << std::endl;
    std::cout << "Nivel actual: " << juego.getNumeroNivel() << std::endl;
    std::cout << "Puntaje: " << juego.getPuntajeTotal() << std::endl;
    std::cout << "Vidas: " << juego.getVidas() << std::endl;

    return 0;
}*/

#include <iostream>
#include "Juego.h"

int main()
{
    Juego juego;

    juego.iniciar();

    std::cout << "CAPA LOGICA" << std::endl;
    std::cout << "Juego iniciado correctamente" << std::endl;

    for (int i = 0; i < 120; i++) {
        juego.actualizar(0.016f);
    }

    std::cout << "Nivel actual: " << juego.getNumeroNivel() << std::endl;
    std::cout << "Puntaje: " << juego.getPuntajeTotal() << std::endl;
    std::cout << "Vidas: " << juego.getVidas() << std::endl;
    std::cout << "Juego activo: " << juego.estaActivo() << std::endl;
    std::cout << "Juego completado: " << juego.estaCompletado() << std::endl;

    std::cout << "La capa logica compila y ejecuta correctamente." << std::endl;

    return 0;
}