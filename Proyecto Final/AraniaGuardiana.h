#ifndef ARANIAGUARDIANA_H
#define ARANIAGUARDIANA_H

#include "Enemigo.h"

#include <map>
#include <queue>
#include <string>

class AraniaGuardiana : public Enemigo
{
public:
    AraniaGuardiana(float x = 0.0f, float y = 0.0f);

    std::string percibir(Jugador* jugador);
    void razonar();
    void actuar(Jugador* jugador);
    void aprender(const std::string& zona);
    bool debeColocarHilo(const std::string& zona) const;

    const std::map<std::string, int>& getMemoriaRutas() const;

private:
    std::map<std::string, int> memoriaRutas;
    std::queue<std::string> acciones;
};

#endif
