#ifndef NIVEL_H
#define NIVEL_H

#include "Jugador.h"
#include "Obstaculo.h"
#include "Recolectable.h"
#include "Plataforma.h"
#include <vector>

class Nivel {
protected:
    Jugador* jugador;
    std::vector<Obstaculo*> obstaculos;
    std::vector<Recolectable*> recolectables;
    std::vector<Plataforma*> plataformas;

    bool completado;

public:
    Nivel();
    virtual ~Nivel();

    virtual void cargarNivel() = 0;
    virtual void actualizar(float deltaTiempo) = 0;
    virtual void verificarColisiones();

    Jugador* getJugador() const;
    bool estaCompletado() const;

    const std::vector<Obstaculo*>& getObstaculos() const;
    const std::vector<Recolectable*>& getRecolectables() const;
    const std::vector<Plataforma*>& getPlataformas() const;
};

#endif