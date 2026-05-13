#include "AraniaGuardiana.h"

AraniaGuardiana::AraniaGuardiana(float x, float y)
    : Enemigo(x, y),
    estadoIA("patrullar"),
    zonaActualJugador("zonaCentral"),
    distanciaJugador(0),
    semillasJugador(0) {
    ancho = 70;
    alto = 60;
    agresividad = 1.2f;
    rangoVision = 180.0f;
    danio = 1;
}

AraniaGuardiana::~AraniaGuardiana() {
}

void AraniaGuardiana::actualizar(float deltaTiempo) {
    Personaje::actualizar(deltaTiempo);
}

void AraniaGuardiana::percibir(Jugador* jugador) {
    if (jugador == nullptr) {
        return;
    }

    distanciaJugador = calcularDistancia(jugador);
    zonaActualJugador = obtenerZonaJugador(jugador);
    semillasJugador = jugador->getSemillas();

    aprender(zonaActualJugador);
}

void AraniaGuardiana::razonar() {
    if (distanciaJugador < rangoVision) {
        estadoIA = "perseguir";
        accionesPendientes.push("perseguir");
    }
    else if (debeColocarHilo(zonaActualJugador)) {
        estadoIA = "bloquear";
        accionesPendientes.push("colocar_hilo");
    }
    else {
        estadoIA = "patrullar";
        accionesPendientes.push("patrullar");
    }

    if (semillasJugador >= 2) {
        agresividad = 1.8f;
    }
}

void AraniaGuardiana::actuar(Jugador* jugador) {
    if (accionesPendientes.empty()) {
        return;
    }

    std::string accion = accionesPendientes.front();
    accionesPendientes.pop();

    if (accion == "perseguir") {
        perseguir(jugador);
    }
    else if (accion == "colocar_hilo") {
        velocidadX = 0;
        velocidadY = 0;
        estado = "colocando hilo";
    }
    else {
        velocidadX = 20;
        velocidadY = 0;
        estado = "patrullando";
    }
}

void AraniaGuardiana::aprender(const std::string& zonaVisitada) {
    rutasMemoria[zonaVisitada]++;
}

std::string AraniaGuardiana::obtenerZonaJugador(Jugador* jugador) const {
    if (jugador == nullptr) {
        return "zonaDesconocida";
    }

    if (jugador->getY() < 200) {
        return "zonaSuperior";
    }
    else if (jugador->getY() < 400) {
        return "zonaCentral";
    }
    else {
        return "zonaInferior";
    }
}

bool AraniaGuardiana::debeColocarHilo(const std::string& zona) const {
    auto it = rutasMemoria.find(zona);

    if (it == rutasMemoria.end()) {
        return false;
    }

    return it->second >= 3;
}

std::string AraniaGuardiana::getEstadoIA() const {
    return estadoIA;
}

int AraniaGuardiana::getVisitasZona(const std::string& zona) const {
    auto it = rutasMemoria.find(zona);

    if (it == rutasMemoria.end()) {
        return 0;
    }

    return it->second;
}