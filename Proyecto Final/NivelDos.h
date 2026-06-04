#ifndef NIVELDOS_H
#define NIVELDOS_H

#include "AraniaGuardiana.h"
#include "Nivel.h"

#include <QRectF>
#include <vector>

class NivelDos : public Nivel
{
public:
    NivelDos(int dificultad, const Character& character);
    ~NivelDos() override;

    void cargarNivel() override;
    void actualizar(float deltaTiempo) override;
    void verificarColisiones() override;

    void moverJugador(float dx, float dy) override;
    void impulsoJugador() override;

    QString getNombre() const override;
    AraniaGuardiana* getArania() const;
    const std::vector<QRectF>& getHilosDinamicos() const;
    QRectF getSalidaRect() const;
    bool salidaEstaAbierta() const;

private:
    float velocidadBase() const;
    void colocarHiloDinamico(const std::string& zona);
    QRectF rectDesdeZona(const std::string& zona) const;

    AraniaGuardiana* arania;
    std::vector<QRectF> hilosDinamicos;
    QRectF salida;
    bool salidaAbierta;
    bool enHilo;
    float mundoAncho;
    float mundoAlto;
    float ataqueCooldown;
    float aprendizajeCooldown;
    int maxHilosDinamicos;
};

#endif
