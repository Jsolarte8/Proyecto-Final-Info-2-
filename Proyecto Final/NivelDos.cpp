#include "NivelDos.h"
#include "SistemaColisiones.h"

#include <QtGlobal>
#include <algorithm>
#include <cmath>
#include <cstdio>

NivelDos::NivelDos(int dificultad, const Character& character)
    : Nivel(dificultad, character),
      arania(nullptr),
      salida(1480.0, 690.0, 90.0, 90.0),
      salidaAbierta(false),
      enHilo(false),
      mundoAncho(1650.0f),
      mundoAlto(900.0f),
      ataqueCooldown(0.0f),
      aprendizajeCooldown(0.0f),
      maxHilosDinamicos(dificultad == 0 ? 3 : (dificultad == 1 ? 5 : 8))
{
}

NivelDos::~NivelDos()
{
    delete arania;
    arania = nullptr;
}

void NivelDos::cargarNivel()
{
    limpiarEntidades();
    delete arania;
    arania = nullptr;
    hilosDinamicos.clear();
    salidaAbierta = false;
    enHilo = false;
    estado = EstadoNivel::EnCurso;

    jugador = new Jugador(80.0f, 90.0f, character.getHealth());
    jugador->setAncho(46.0f);
    jugador->setAlto(54.0f);

    arania = new AraniaGuardiana(1180.0f, 610.0f);
    arania->setAgresividad(dificultad == 0 ? 0.85f : (dificultad == 1 ? 1.08f : 1.35f));
    arania->setDanio(dificultad == 0 ? 7 : (dificultad == 1 ? 10 : 14));

    recolectables.push_back(new Recolectable("semilla", 300.0f, 190.0f, 34.0f, 34.0f, 60));
    recolectables.push_back(new Recolectable("semilla", 840.0f, 420.0f, 34.0f, 34.0f, 60));
    recolectables.push_back(new Recolectable("semilla", 1350.0f, 240.0f, 34.0f, 34.0f, 60));

    obstaculos.push_back(new Obstaculo("hilo", 470.0f, 70.0f, 28.0f, 520.0f, 0, 0.45f, false));
    obstaculos.push_back(new Obstaculo("hilo", 730.0f, 330.0f, 520.0f, 28.0f, 0, 0.45f, false));
    obstaculos.push_back(new Obstaculo("hilo", 1120.0f, 130.0f, 28.0f, 520.0f, 0, 0.45f, false));

    if (dificultad >= 1) {
        obstaculos.push_back(new Obstaculo("hilo", 230.0f, 620.0f, 430.0f, 28.0f, 0, 0.45f, false));
    }
    if (dificultad == 2) {
        obstaculos.push_back(new Obstaculo("hilo", 980.0f, 640.0f, 470.0f, 28.0f, 0, 0.45f, false));
    }
}

void NivelDos::actualizar(float deltaTiempo)
{
    if (!jugador || !arania || estado != EstadoNivel::EnCurso) {
        return;
    }

    ataqueCooldown = std::max(0.0f, ataqueCooldown - deltaTiempo);
    aprendizajeCooldown = std::max(0.0f, aprendizajeCooldown - deltaTiempo);

    jugador->actualizar(deltaTiempo);
    jugador->setX(std::clamp(jugador->getX(), 20.0f, mundoAncho - jugador->getAncho() - 20.0f));
    jugador->setY(std::clamp(jugador->getY(), 20.0f, mundoAlto - jugador->getAlto() - 20.0f));

    if (aprendizajeCooldown <= 0.0f) {
        const std::string zona = arania->percibir(jugador);
        arania->aprender(zona);
        if (arania->debeColocarHilo(zona) && static_cast<int>(hilosDinamicos.size()) < maxHilosDinamicos) {
            colocarHiloDinamico(zona);
        }
        aprendizajeCooldown = dificultad == 2 ? 0.55f : 0.8f;
    }

    arania->setAgresividad((dificultad == 0 ? 0.85f : (dificultad == 1 ? 1.08f : 1.35f))
                           + jugador->getSemillas() * 0.22f);
    arania->actuar(jugador);
    arania->actualizar(deltaTiempo);
    arania->setX(std::clamp(arania->getX(), 20.0f, mundoAncho - arania->getAncho() - 20.0f));
    arania->setY(std::clamp(arania->getY(), 20.0f, mundoAlto - arania->getAlto() - 20.0f));

    verificarColisiones();

    if (jugador->getVidas() <= 0) {
        estado = EstadoNivel::Perdido;
    }
}

void NivelDos::verificarColisiones()
{
    enHilo = false;

    for (Recolectable* recolectable : recolectables) {
        recolectable->recoger(jugador);
    }

    for (Obstaculo* obstaculo : obstaculos) {
        if (SistemaColisiones::colisionan(jugador, obstaculo) && obstaculo->getTipo() == "hilo") {
            enHilo = true;
        }
    }

    for (const QRectF& hilo : hilosDinamicos) {
        if (jugador->rect().intersects(hilo)) {
            enHilo = true;
        }
    }

    salidaAbierta = jugador->getSemillas() >= 3;

    if (salidaAbierta && jugador->rect().intersects(salida)) {
        estado = EstadoNivel::Completado;
        return;
    }

    if (jugador->rect().intersects(arania->rect()) && ataqueCooldown <= 0.0f) {
        arania->atacar(jugador);
        ataqueCooldown = dificultad == 2 ? 0.75f : 1.1f;
    }
}

void NivelDos::moverJugador(float dx, float dy)
{
    if (!jugador || estado != EstadoNivel::EnCurso) {
        return;
    }

    float longitud = std::sqrt(dx * dx + dy * dy);
    if (longitud > 1.0f) {
        dx /= longitud;
        dy /= longitud;
    }

    float velocidad = velocidadBase();
    if (enHilo) {
        velocidad *= 0.45f;
    }

    jugador->setVelocidadX(dx * velocidad);
    jugador->setVelocidadY(dy * velocidad);
    jugador->setEstado(dx == 0.0f && dy == 0.0f ? "idle" : "walk");
}

void NivelDos::impulsoJugador()
{
    if (!jugador || estado != EstadoNivel::EnCurso) {
        return;
    }

    float vx = jugador->getVelocidadX();
    float vy = jugador->getVelocidadY();
    if (std::abs(vx) < 1.0f && std::abs(vy) < 1.0f) {
        vx = velocidadBase();
    }

    jugador->setVelocidadX(vx * 2.15f);
    jugador->setVelocidadY(vy * 2.15f);
    jugador->setEstado("jump");
}

QString NivelDos::getNombre() const
{
    return "La Red de la Araña Guardiana";
}

AraniaGuardiana* NivelDos::getArania() const { return arania; }
const std::vector<QRectF>& NivelDos::getHilosDinamicos() const { return hilosDinamicos; }
QRectF NivelDos::getSalidaRect() const { return salida; }
bool NivelDos::salidaEstaAbierta() const { return salidaAbierta; }

float NivelDos::velocidadBase() const
{
    return 70.0f + character.getSpeed() * 8.2f;
}

void NivelDos::colocarHiloDinamico(const std::string& zona)
{
    const QRectF rect = rectDesdeZona(zona);
    for (const QRectF& existente : hilosDinamicos) {
        if (existente.intersects(rect.adjusted(-15, -15, 15, 15))) {
            return;
        }
    }
    hilosDinamicos.push_back(rect);
}

QRectF NivelDos::rectDesdeZona(const std::string& zona) const
{
    int zonaX = 0;
    int zonaY = 0;
    std::sscanf(zona.c_str(), "zona_%d_%d", &zonaX, &zonaY);
    return QRectF(zonaX * 120.0 + 35.0, zonaY * 100.0 + 38.0, 96.0, 18.0);
}
