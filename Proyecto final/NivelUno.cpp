#include "NivelUno.h"

NivelUno::NivelUno()
    : Nivel(),
    temporizador(nullptr),
    fisicaParabolica(nullptr),
    fisicaElastica(nullptr),
    fisicaFriccion(nullptr),
    posicionMeta(1000.0f) {
}

NivelUno::~NivelUno() {
    delete temporizador;
    delete fisicaParabolica;
    delete fisicaElastica;
    delete fisicaFriccion;
}

void NivelUno::cargarNivel() {
    jugador = new Jugador(50, 300);

    temporizador = new Temporizador(60.0f);
    temporizador->iniciar();

    fisicaParabolica = new FisicaParabolica(jugador);
    fisicaElastica = new FisicaElastica(jugador);
    fisicaFriccion = new FisicaFriccion(jugador);

    obstaculos.push_back(new Obstaculo("piedra", 250, 320, 50, 50));
    obstaculos.push_back(new Obstaculo("raiz", 450, 320, 60, 40));
    obstaculos.push_back(new Obstaculo("charco", 650, 340, 90, 20));

    recolectables.push_back(new Recolectable("nectar", 200, 250));
    recolectables.push_back(new Recolectable("nectar", 500, 230));
    recolectables.push_back(new Recolectable("nectar", 750, 220));

    plataformas.push_back(new Plataforma("hoja", 0, 380, 1200, 30));
}

void NivelUno::actualizar(float deltaTiempo) {
    if (jugador == nullptr) {
        return;
    }

    temporizador->actualizar(deltaTiempo);

    fisicaElastica->calcular(deltaTiempo);
    fisicaParabolica->calcular(deltaTiempo);
    fisicaFriccion->calcular(deltaTiempo);

    jugador->actualizar(deltaTiempo);

    for (Obstaculo* obstaculo : obstaculos) {
        obstaculo->actualizar(deltaTiempo);
    }

    for (Recolectable* recolectable : recolectables) {
        recolectable->actualizar(deltaTiempo);
    }

    for (Plataforma* plataforma : plataformas) {
        plataforma->actualizar(deltaTiempo);
    }

    verificarColisiones();
    validarMeta();

    if (temporizador->tiempoTerminado()) {
        jugador->recibirDanio(1);
        temporizador->reiniciar();
    }
}

void NivelUno::validarMeta() {
    if (jugador != nullptr && jugador->getX() >= posicionMeta) {
        completado = true;
    }
}

bool NivelUno::tiempoTerminado() const {
    if (temporizador == nullptr) {
        return false;
    }

    return temporizador->tiempoTerminado();
}