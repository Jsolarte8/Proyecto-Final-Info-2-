#ifndef NIVELUNO_H
#define NIVELUNO_H

#include "FisicaElastica.h"
#include "FisicaFriccion.h"
#include "Nivel.h"
#include "Temporizador.h"

#include <QRectF>

class NivelUno : public Nivel
{
public:
    NivelUno(int dificultad, const Character& character);

    void cargarNivel() override;
    void actualizar(float deltaTiempo) override;
    void verificarColisiones() override;

    void moverJugador(float dx, float dy) override;
    void saltarJugador() override;
    void cargarPertiga(float deltaTiempo) override;
    void usarPertiga() override;

    QString getNombre() const override;
    float getTiempoRestante() const;
    QRectF getMetaRect() const;
    float getSueloY() const;
    bool estaEnCharco() const;

private:
    float velocidadHorizontalBase() const;

    Temporizador temporizador;
    FisicaElastica fisicaElastica;
    FisicaFriccion fisicaFriccion;
    QRectF meta;
    float sueloY;
    float mundoAncho;
    float invulnerabilidad;
    bool enCharco;
    float direccionActual;
};

#endif
