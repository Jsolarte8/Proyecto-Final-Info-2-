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
      maxHilosDinamicos(dificultad == 0 ? 2 : (dificultad == 1 ? 7 : 14))
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
    arania->setAgresividad(dificultad == 0 ? 0.72f : (dificultad == 1 ? 1.18f : 1.72f));
    arania->setDanio(dificultad == 0 ? 6 : (dificultad == 1 ? 12 : 18));

    recolectables.push_back(new Recolectable("semilla", 300.0f, 190.0f, 34.0f, 34.0f, 60));
    recolectables.push_back(new Recolectable("semilla", 840.0f, 420.0f, 34.0f, 34.0f, 60));
    recolectables.push_back(new Recolectable("semilla", 1350.0f, 240.0f, 34.0f, 34.0f, 60));

    const float frenoHilo = dificultad == 0 ? 0.58f : (dificultad == 1 ? 0.42f : 0.28f);

    obstaculos.push_back(new Obstaculo("hilo", 470.0f, 70.0f, 30.0f, 520.0f, 0, frenoHilo, false));
    obstaculos.push_back(new Obstaculo("hilo", 730.0f, 330.0f, 520.0f, 30.0f, 0, frenoHilo, false));
    obstaculos.push_back(new Obstaculo("hilo", 1120.0f, 130.0f, 30.0f, 520.0f, 0, frenoHilo, false));

    if (dificultad >= 1) {
        obstaculos.push_back(new Obstaculo("hilo", 230.0f, 620.0f, 430.0f, 30.0f, 0, frenoHilo, false));
    }
    if (dificultad == 2) {
        obstaculos.push_back(new Obstaculo("hilo", 980.0f, 640.0f, 470.0f, 30.0f, 0, frenoHilo, false));
        obstaculos.push_back(new Obstaculo("hilo", 250.0f, 240.0f, 30.0f, 360.0f, 0, frenoHilo, false));
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
        aprendizajeCooldown = dificultad == 0 ? 1.15f : (dificultad == 1 ? 0.65f : 0.35f);
    }

    arania->setAgresividad((dificultad == 0 ? 0.72f : (dificultad == 1 ? 1.18f : 1.72f))
                           + jugador->getSemillas() * (dificultad == 2 ? 0.34f : 0.22f));
    const std::string accionArania = arania->actuar(jugador);
    const std::string prefijoColocarHilo = "colocar_hilo:";
    if (accionArania.rfind(prefijoColocarHilo, 0) == 0
        && static_cast<int>(hilosDinamicos.size()) < maxHilosDinamicos) {
        colocarHiloDinamico(accionArania.substr(prefijoColocarHilo.size()));
    }
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
        ataqueCooldown = dificultad == 0 ? 1.45f : (dificultad == 1 ? 1.0f : 0.58f);
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
        velocidad *= dificultad == 0 ? 0.58f : (dificultad == 1 ? 0.42f : 0.28f);
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
    const float factorDificultad = dificultad == 0 ? 1.08f : (dificultad == 1 ? 1.0f : 0.90f);
    return (70.0f + character.getSpeed() * 8.2f) * factorDificultad;
}

void NivelDos::colocarHiloDinamico(const std::string& zona)
{
    const QRectF rect = rectDesdeZona(zona);
    for (const QRectF& existente : hilosDinamicos) {
        if (existente.intersects(rect.adjusted(-12, -12, 12, 12))) {
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
    const bool vertical = (zonaX + zonaY) % 3 == 0;
    const qreal anchoHilo = vertical ? 34.0 : 170.0;
    const qreal altoHilo = vertical ? 180.0 : 32.0;
    const qreal x = std::clamp<qreal>(zonaX * 120.0 + 28.0, 20.0, mundoAncho - anchoHilo - 20.0);
    const qreal y = std::clamp<qreal>(zonaY * 100.0 + 34.0, 20.0, mundoAlto - altoHilo - 20.0);
    return QRectF(x, y, anchoHilo, altoHilo);
}
