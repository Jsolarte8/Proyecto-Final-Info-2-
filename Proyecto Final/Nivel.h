#ifndef NIVEL_H
#define NIVEL_H

#include "Character.h"
#include "Jugador.h"
#include "Obstaculo.h"
#include "Plataforma.h"
#include "Recolectable.h"

#include <vector>

enum class EstadoNivel
{
    EnCurso,
    Completado,
    Perdido
};

class Nivel
{
public:
    Nivel(int dificultad, const Character& character);
    virtual ~Nivel();

    virtual void cargarNivel() = 0;
    virtual void actualizar(float deltaTiempo);
    virtual void verificarColisiones();

    virtual void moverJugador(float dx, float dy);
    virtual void saltarJugador();
    virtual void cargarPertiga(float deltaTiempo);
    virtual void usarPertiga();
    virtual void impulsoJugador();

    Jugador* getJugador() const;
    const std::vector<Obstaculo*>& getObstaculos() const;
    const std::vector<Recolectable*>& getRecolectables() const;
    const std::vector<Plataforma*>& getPlataformas() const;
    EstadoNivel getEstado() const;
    int getDificultad() const;
    virtual QString getNombre() const = 0;

protected:
    void limpiarEntidades();

    Jugador* jugador;
    std::vector<Obstaculo*> obstaculos;
    std::vector<Recolectable*> recolectables;
    std::vector<Plataforma*> plataformas;
    int dificultad;
    Character character;
    EstadoNivel estado;
};

#endif
