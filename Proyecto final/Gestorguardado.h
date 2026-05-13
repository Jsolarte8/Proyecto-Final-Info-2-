#ifndef GESTORGUARDADO_H
#define GESTORGUARDADO_H

#include <string>

class GestorGuardado {
private:
    int nivelGuardado;
    int puntajeGuardado;
    int aprendizajeArania;

public:
    GestorGuardado();

    void guardarProgreso(const std::string& nombreArchivo,
                         int nivel,
                         int puntaje,
                         int aprendizaje);

    void cargarProgreso(const std::string& nombreArchivo);

    int getNivelGuardado() const;
    int getPuntajeGuardado() const;
    int getAprendizajeArania() const;
};

#endif