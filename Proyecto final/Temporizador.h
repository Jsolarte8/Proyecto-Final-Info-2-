#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H

class Temporizador {
private:
    float tiempoActual;
    float tiempoLimite;
    bool activo;

public:
    Temporizador(float tiempoLimite = 60.0f);

    void iniciar();
    void detener();
    void reiniciar();
    void actualizar(float deltaTiempo);

    bool tiempoTerminado() const;

    float getTiempoActual() const;
    float getTiempoRestante() const;
};

#endif