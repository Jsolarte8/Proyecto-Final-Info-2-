#include "AraniaGuardiana.h"

#include <QtGlobal>
#include <sstream>

AraniaGuardiana::AraniaGuardiana(float x, float y)
    : Enemigo(x, y, 120)
{
    ancho = 70.0f;
    alto = 58.0f;
    rangoVision = 900.0f;
    danio = 10;
}

std::string AraniaGuardiana::percibir(Jugador* jugador)
{
    if (!jugador) {
        return "zona_0_0";
    }

    const int zonaX = static_cast<int>(jugador->getX() / 120.0f);
    const int zonaY = static_cast<int>(jugador->getY() / 100.0f);
    std::ostringstream stream;
    stream << "zona_" << zonaX << "_" << zonaY;
    return stream.str();
}

void AraniaGuardiana::razonar()
{
    for (const auto& entrada : memoriaRutas) {
        if (entrada.second >= 3) {
            acciones.push("colocar_hilo:" + entrada.first);
        }
    }

    if (acciones.empty()) {
        acciones.push("perseguir");
    }
}

void AraniaGuardiana::actuar(Jugador* jugador)
{
    if (acciones.empty()) {
        razonar();
    }

    const std::string accion = acciones.front();
    acciones.pop();

    Q_UNUSED(accion);
    perseguir(jugador);
}

void AraniaGuardiana::aprender(const std::string& zona)
{
    ++memoriaRutas[zona];
}

bool AraniaGuardiana::debeColocarHilo(const std::string& zona) const
{
    const auto it = memoriaRutas.find(zona);
    return it != memoriaRutas.end() && it->second >= 3;
}

const std::map<std::string, int>& AraniaGuardiana::getMemoriaRutas() const
{
    return memoriaRutas;
}
