#include "Jugador.h"

Jugador::Jugador(float x, float y)
    : Personaje(x, y, 40, 60),
    puntaje(0),
    nectar(0),
    semillas(0),
    energiaPertiga(0),
    enAire(false) {
    estado = "quieto";
}

Jugador::~Jugador() {
}

void Jugador::actualizar(float deltaTiempo) {
    Personaje::actualizar(deltaTiempo);

    if (enAire) {
        estado = "saltando";
    }
}

void Jugador::cargarPertiga(float cantidad) {
    energiaPertiga += cantidad;

    if (energiaPertiga > 100) {
        energiaPertiga = 100;
    }

    estado = "cargando pertiga";
}

void Jugador::usarPertiga() {
    if (energiaPertiga > 0) {
        enAire = true;
        estado = "saltando";
    }
}

void Jugador::caer() {
    enAire = true;
    estado = "cayendo";
}

void Jugador::aterrizar() {
    enAire = false;
    energiaPertiga = 0;
    estado = "quieto";
}

void Jugador::aumentarPuntaje(int valor) {
    puntaje += valor;
}

void Jugador::recolectarNectar() {
    nectar++;
    puntaje += 10;
}

void Jugador::recolectarSemilla() {
    semillas++;
}

int Jugador::getPuntaje() const {
    return puntaje;
}

int Jugador::getNectar() const {
    return nectar;
}

int Jugador::getSemillas() const {
    return semillas;
}

float Jugador::getEnergiaPertiga() const {
    return energiaPertiga;
}

void Jugador::setEnergiaPertiga(float energia) {
    energiaPertiga = energia;
}

bool Jugador::estaEnAire() const {
    return enAire;
}