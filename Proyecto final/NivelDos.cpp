#include "NivelDos.h"
#include "SistemaColisiones.h"

NivelDos::NivelDos()
    : Nivel(),
    arania(nullptr),
    fisicaParabolica(nullptr),
    fisicaElastica(nullptr),
    fisicaFriccion(nullptr),
    salidaActiva(false),
    salidaX(900),
    salidaY(100) {
}

NivelDos::~NivelDos() {
    delete arania;
    delete fisicaParabolica;
    delete fisicaElastica;
    delete fisicaFriccion;
}

void NivelDos::cargarNivel() {
    jugador = new Jugador(100, 350);
    arania = new AraniaGuardiana(500, 300);

    fisicaParabolica = new FisicaParabolica(jugador);
    fisicaElastica = new FisicaElastica(jugador);
    fisicaFriccion = new FisicaFriccion(jugador);

    recolectables.push_back(new Recolectable("semilla", 200, 120));
    recolectables.push_back(new Recolectable("semilla", 650, 180));
    recolectables.push_back(new Recolectable("semilla", 400, 450));

    obstaculos.push_back(new Obstaculo("hilo_pegajoso", 350, 250, 120, 40));
    obstaculos.push_back(new Obstaculo("hilo_pegajoso", 700, 350, 120, 40));

    plataformas.push_back(new Plataforma("telarana", 0, 0, 1000, 600));
}

void NivelDos::actualizar(float deltaTiempo) {
    if (jugador == nullptr || arania == nullptr) {
        return;
    }

    fisicaElastica->calcular(deltaTiempo);
    fisicaParabolica->calcular(deltaTiempo);
    fisicaFriccion->calcular(deltaTiempo);

    jugador->actualizar(deltaTiempo);

    arania->percibir(jugador);
    arania->razonar();
    arania->actuar(jugador);
    arania->actualizar(deltaTiempo);

    for (Obstaculo* obstaculo : obstaculos) {
        obstaculo->actualizar(deltaTiempo);
    }

    for (Recolectable* recolectable : recolectables) {
        recolectable->actualizar(deltaTiempo);
    }

    verificarColisiones();
    validarSalida();
}

void NivelDos::verificarColisiones() {
    Nivel::verificarColisiones();

    if (SistemaColisiones::verificarColision(jugador, arania)) {
        arania->atacar(jugador);
    }
}

void NivelDos::validarSalida() {
    if (jugador == nullptr) {
        return;
    }

    if (jugador->getSemillas() >= 3) {
        salidaActiva = true;
    }

    bool cercaSalida = jugador->getX() >= salidaX - 30 &&
                       jugador->getX() <= salidaX + 30 &&
                       jugador->getY() >= salidaY - 30 &&
                       jugador->getY() <= salidaY + 30;

    if (salidaActiva && cercaSalida) {
        completado = true;
    }
}

AraniaGuardiana* NivelDos::getArania() const {
    return arania;
}