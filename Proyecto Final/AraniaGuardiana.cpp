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
    while (!acciones.empty()) {
        acciones.pop();
    }

    std::string zonaMasVisitada;
    int visitasZonaMasVisitada = 0;
    for (const auto& entrada : memoriaRutas) {
        if (entrada.second >= 2 && entrada.second > visitasZonaMasVisitada) {
            zonaMasVisitada = entrada.first;
            visitasZonaMasVisitada = entrada.second;
        }
    }

    if (!zonaMasVisitada.empty()) {
        acciones.push("colocar_hilo:" + zonaMasVisitada);
    }

    acciones.push("perseguir");
}

std::string AraniaGuardiana::actuar(Jugador* jugador)
{
    if (acciones.empty()) {
        razonar();
    }

    const std::string accion = acciones.front();
    acciones.pop();

    if (accion == "perseguir") {
        perseguir(jugador);
    } else if (accion.rfind("colocar_hilo:", 0) == 0) {
        memoriaRutas[accion.substr(13)] = 0;
    }

    return accion;
}

void AraniaGuardiana::aprender(const std::string& zona)
{
    ++memoriaRutas[zona];
}

bool AraniaGuardiana::debeColocarHilo(const std::string& zona) const
{
    const auto it = memoriaRutas.find(zona);
    return it != memoriaRutas.end() && it->second >= 2;
}

const std::map<std::string, int>& AraniaGuardiana::getMemoriaRutas() const
{
    return memoriaRutas;
}
