#include "Nivel.h"
#include "SistemaColisiones.h"

Nivel::Nivel()
    : jugador(nullptr),
    completado(false) {
}

Nivel::~Nivel() {
    delete jugador;

    for (Obstaculo* obstaculo : obstaculos) {
        delete obstaculo;
    }

    for (Recolectable* recolectable : recolectables) {
        delete recolectable;
    }

    for (Plataforma* plataforma : plataformas) {
        delete plataforma;
    }

    obstaculos.clear();
    recolectables.clear();
    plataformas.clear();
}

void Nivel::verificarColisiones() {
    if (jugador == nullptr) {
        return;
    }

    for (Obstaculo* obstaculo : obstaculos) {
        if (SistemaColisiones::verificarColision(jugador, obstaculo)) {
            obstaculo->afectar(jugador);
        }
    }

    for (Recolectable* recolectable : recolectables) {
        if (SistemaColisiones::verificarColision(jugador, recolectable)) {
            recolectable->recoger(jugador);
        }
    }

    for (Plataforma* plataforma : plataformas) {
        if (SistemaColisiones::verificarColision(jugador, plataforma)) {
            plataforma->soportarJugador(jugador);
        }
    }
}

Jugador* Nivel::getJugador() const {
    return jugador;
}

bool Nivel::estaCompletado() const {
    return completado;
}

const std::vector<Obstaculo*>& Nivel::getObstaculos() const {
    return obstaculos;
}

const std::vector<Recolectable*>& Nivel::getRecolectables() const {
    return recolectables;
}

const std::vector<Plataforma*>& Nivel::getPlataformas() const {
    return plataformas;
}