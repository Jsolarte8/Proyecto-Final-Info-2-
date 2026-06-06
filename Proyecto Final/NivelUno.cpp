#include "NivelUno.h"
#include "SistemaColisiones.h"

#include <QtGlobal>
#include <algorithm>
#include <vector>

namespace {
bool jugadorSobreSuperficie(const Jugador* jugador, const std::vector<Plataforma*>& plataformas, float sueloY)
{
    if (!jugador) {
        return false;
    }

    const float base = jugador->getY() + jugador->getAlto();
    if (qAbs(base - sueloY) <= 2.0f) {
        return true;
    }

    const float left = jugador->getX() + 5.0f;
    const float right = jugador->getX() + jugador->getAncho() - 5.0f;
    for (const Plataforma* plataforma : plataformas) {
        if (!plataforma) {
            continue;
        }

        const float top = plataforma->getY();
        const float overlap = std::min(right, plataforma->getX() + plataforma->getAncho()) - std::max(left, plataforma->getX());
        if (overlap > 6.0f && qAbs(base - top) <= 2.5f) {
            return true;
        }
    }

    return false;
}
}

NivelUno::NivelUno(int dificultad, const Character& character)
    : Nivel(dificultad, character),
      temporizador(90.0f),
      fisicaParabolica(1450.0f),
      fisicaElastica(7.5f, 0.82f),
      fisicaFriccion(0.48f),
      meta(2260.0, 386.0, 112.0, 124.0),
      sueloY(510.0f),
      mundoAncho(2400.0f),
      invulnerabilidad(0.0f),
      enCharco(false),
      direccionActual(1.0f)
{
}

void NivelUno::cargarNivel()
{
    limpiarEntidades();
    estado = EstadoNivel::EnCurso;

    jugador = new Jugador(60.0f, sueloY - 64.0f, character.getHealth());

    plataformas.push_back(new Plataforma(0.0f, sueloY, mundoAncho, 42.0f, "hoja"));
    plataformas.push_back(new Plataforma(430.0f, 405.0f, 170.0f, 24.0f, "hoja_alta"));
    plataformas.push_back(new Plataforma(980.0f, 360.0f, 160.0f, 24.0f, "hoja_alta"));
    plataformas.push_back(new Plataforma(1550.0f, 400.0f, 190.0f, 24.0f, "hoja_alta"));

    const int danioBase = dificultad == 0 ? 7 : (dificultad == 1 ? 11 : 16);
    const float friccionCharco = dificultad == 0 ? 0.62f : (dificultad == 1 ? 0.44f : 0.30f);
    fisicaFriccion.setCoeficiente(friccionCharco);

    obstaculos.push_back(new Obstaculo("piedra", 335.0f, sueloY - 62.0f, 74.0f, 62.0f, danioBase, 1.0f, false));
    obstaculos.push_back(new Obstaculo("raiz", 720.0f, sueloY - 54.0f, 116.0f, 54.0f, danioBase + 2, 1.0f, false));
    obstaculos.push_back(new Obstaculo("charco", 1160.0f, sueloY - 30.0f, 220.0f, 30.0f, 0, friccionCharco, false));
    obstaculos.push_back(new Obstaculo("piedra", 1790.0f, sueloY - 66.0f, 82.0f, 66.0f, danioBase + 1, 1.0f, false));

    if (dificultad >= 1) {
        obstaculos.push_back(new Obstaculo("raiz", 1470.0f, sueloY - 52.0f, 112.0f, 52.0f, danioBase + 1, 1.0f, false));
    }
    if (dificultad == 2) {
        obstaculos.push_back(new Obstaculo("charco", 520.0f, sueloY - 30.0f, 210.0f, 30.0f, 0, friccionCharco, false));
        obstaculos.push_back(new Obstaculo("piedra", 2040.0f, sueloY - 62.0f, 78.0f, 62.0f, danioBase + 2, 1.0f, false));
    }

    const int gotas = dificultad == 0 ? 2 : (dificultad == 1 ? 4 : 7);
    for (int i = 0; i < gotas; ++i) {
        obstaculos.push_back(new Obstaculo("gota",
                                           580.0f + i * 285.0f,
                                           230.0f + (i % 2) * 70.0f,
                                           56.0f,
                                           72.0f,
                                           dificultad == 0 ? 9 : (dificultad == 1 ? 13 : 19),
                                           1.0f,
                                           true));
    }

    recolectables.push_back(new Recolectable("nectar", 250.0f, sueloY - 88.0f, 32.0f, 32.0f, 10));
    recolectables.push_back(new Recolectable("nectar", 520.0f, 350.0f, 32.0f, 32.0f, 10));
    recolectables.push_back(new Recolectable("nectar", 1070.0f, 305.0f, 32.0f, 32.0f, 15));
    recolectables.push_back(new Recolectable("nectar", 1605.0f, 345.0f, 32.0f, 32.0f, 15));
    recolectables.push_back(new Recolectable("nectar", 2050.0f, sueloY - 90.0f, 32.0f, 32.0f, 20));

    const float tiempo = dificultad == 0 ? 125.0f : (dificultad == 1 ? 85.0f : 58.0f);
    temporizador.iniciar(tiempo);
}

void NivelUno::actualizar(float deltaTiempo)
{
    if (!jugador || estado != EstadoNivel::EnCurso) {
        return;
    }

    temporizador.actualizar(deltaTiempo);
    invulnerabilidad = std::max(0.0f, invulnerabilidad - deltaTiempo);

    for (Obstaculo* obstaculo : obstaculos) {
        obstaculo->actualizar(deltaTiempo);
    }

    if (jugador->estaEnAire()) {
        fisicaParabolica.iniciar(jugador->getX(),
                                 jugador->getY(),
                                 jugador->getVelocidadX(),
                                 jugador->getVelocidadY());
        fisicaParabolica.calcular(deltaTiempo);
        jugador->setPosicion(fisicaParabolica.getX(), fisicaParabolica.getY());
        jugador->setVelocidadY(fisicaParabolica.getVelocidadY());
    } else {
        jugador->actualizar(deltaTiempo);
    }
    jugador->setX(std::clamp(jugador->getX(), 0.0f, mundoAncho - jugador->getAncho()));

    if (jugador->getY() + jugador->getAlto() >= sueloY) {
        jugador->aterrizar(sueloY);
    }

    verificarColisiones();

    if (!jugador->estaEnAire() && !jugadorSobreSuperficie(jugador, plataformas, sueloY)) {
        jugador->setEnAire(true);
        jugador->setVelocidadY(qMax(90.0f, jugador->getVelocidadY()));
        jugador->setEstado("jump");
    }

    if (temporizador.agotado() || jugador->getVidas() <= 0) {
        estado = EstadoNivel::Perdido;
    }
}

void NivelUno::verificarColisiones()
{
    enCharco = false;

    for (Plataforma* plataforma : plataformas) {
        SistemaColisiones::resolverJugadorPlataforma(jugador, plataforma);
    }

    for (Recolectable* recolectable : recolectables) {
        recolectable->recoger(jugador);
    }

    for (Obstaculo* obstaculo : obstaculos) {
        if (!obstaculo->estaActivo() || !SistemaColisiones::colisionan(jugador, obstaculo)) {
            continue;
        }

        if (obstaculo->getTipo() == "charco") {
            enCharco = true;
            continue;
        }

        if (obstaculo->getTipo() == "gota") {
            obstaculo->afectar(jugador);
            invulnerabilidad = 0.6f;
            continue;
        }

        if (invulnerabilidad <= 0.0f) {
            obstaculo->afectar(jugador);
            jugador->setVelocidadX(-direccionActual * 120.0f);
            invulnerabilidad = 1.0f;
        }
    }

    if (jugador->rect().intersects(meta)) {
        estado = EstadoNivel::Completado;
    }
}

void NivelUno::moverJugador(float dx, float dy)
{
    Q_UNUSED(dy);
    if (!jugador || estado != EstadoNivel::EnCurso) {
        return;
    }

    if (dx != 0.0f) {
        direccionActual = dx > 0.0f ? 1.0f : -1.0f;
    }

    if (jugador->getEstado() == "charge" && !jugador->estaEnAire()) {
        jugador->setVelocidadX(0.0f);
        return;
    }

    float velocidad = velocidadHorizontalBase();
    if (enCharco) {
        fisicaFriccion.aplicar(velocidad);
        fisicaFriccion.calcular(0.0f);
        velocidad = fisicaFriccion.getVelocidadResultado();
    }

    jugador->setVelocidadX(dx * velocidad);
    if (!jugador->estaEnAire() && jugador->getEstado() != "charge") {
        jugador->setEstado(dx == 0.0f ? "idle" : "walk");
    }
}

void NivelUno::saltarJugador()
{
    if (!jugador || jugador->estaEnAire() || estado != EstadoNivel::EnCurso) {
        return;
    }

    jugador->setVelocidadY(-430.0f - character.getJump() * 7.0f);
    fisicaParabolica.iniciar(jugador->getX(),
                             jugador->getY(),
                             jugador->getVelocidadX(),
                             jugador->getVelocidadY());
    jugador->setEnAire(true);
    jugador->setEstado("jump");
}

void NivelUno::cargarPertiga(float deltaTiempo)
{
    if (jugador && !jugador->estaEnAire() && estado == EstadoNivel::EnCurso) {
        jugador->cargarPertiga(deltaTiempo + character.getPoleControl() * 0.0009f);
    }
}

void NivelUno::usarPertiga()
{
    if (!jugador || jugador->estaEnAire() || estado != EstadoNivel::EnCurso) {
        return;
    }

    const float energia = jugador->usarPertiga();
    fisicaElastica.cargar(energia + character.getPoleControl());
    const float fuerza = fisicaElastica.soltar();
    jugador->setVelocidadX(direccionActual * (210.0f + fuerza * 0.45f));
    jugador->setVelocidadY(-390.0f - fuerza * 0.35f - character.getJump() * 4.0f);
    fisicaParabolica.iniciar(jugador->getX(),
                             jugador->getY(),
                             jugador->getVelocidadX(),
                             jugador->getVelocidadY());
}

QString NivelUno::getNombre() const
{
    return "La Pista de las Hojas Altas";
}

float NivelUno::getTiempoRestante() const { return temporizador.getTiempoRestante(); }
QRectF NivelUno::getMetaRect() const { return meta; }
float NivelUno::getSueloY() const { return sueloY; }
bool NivelUno::estaEnCharco() const { return enCharco; }

float NivelUno::velocidadHorizontalBase() const
{
    const float factorDificultad = dificultad == 0 ? 1.08f : (dificultad == 1 ? 1.0f : 0.92f);
    return (80.0f + character.getSpeed() * 9.5f) * factorDificultad;
}
