#include "GestorGuardado.h"
#include <fstream>

GestorGuardado::GestorGuardado()
    : nivelGuardado(1),
    puntajeGuardado(0),
    aprendizajeArania(0) {
}

void GestorGuardado::guardarProgreso(const std::string& nombreArchivo,
                                     int nivel,
                                     int puntaje,
                                     int aprendizaje) {
    std::ofstream archivo(nombreArchivo);

    if (archivo.is_open()) {
        archivo << nivel << std::endl;
        archivo << puntaje << std::endl;
        archivo << aprendizaje << std::endl;
        archivo.close();
    }
}

void GestorGuardado::cargarProgreso(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);

    if (archivo.is_open()) {
        archivo >> nivelGuardado;
        archivo >> puntajeGuardado;
        archivo >> aprendizajeArania;
        archivo.close();
    }
}

int GestorGuardado::getNivelGuardado() const {
    return nivelGuardado;
}

int GestorGuardado::getPuntajeGuardado() const {
    return puntajeGuardado;
}

int GestorGuardado::getAprendizajeArania() const {
    return aprendizajeArania;
}