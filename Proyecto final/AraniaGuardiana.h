#ifndef ARANIAGUARDIANA_H
#define ARANIAGUARDIANA_H

#include "Enemigo.h"
#include <map>
#include <queue>


class AraniaGuardiana : public Enemigo {
private:
    std::map<std::string, int> rutasMemoria;
    std::queue<std::string> accionesPendientes;

    std::string estadoIA;
    std::string zonaActualJugador;

    float distanciaJugador;
    int semillasJugador;

public:
    AraniaGuardiana(float x = 0, float y = 0);

    virtual ~AraniaGuardiana();

    virtual void actualizar(float deltaTiempo) override;

    void percibir(Jugador* jugador);
    void razonar();
    void actuar(Jugador* jugador);
    void aprender(const std::string& zonaVisitada);

    std::string obtenerZonaJugador(Jugador* jugador) const;

    bool debeColocarHilo(const std::string& zona) const;

    std::string getEstadoIA() const;
    int getVisitasZona(const std::string& zona) const;
};

#endif