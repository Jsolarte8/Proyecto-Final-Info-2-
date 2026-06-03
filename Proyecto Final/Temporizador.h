#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H

class Temporizador
{
public:
    Temporizador(float segundos = 90.0f);

    void iniciar(float segundos);
    void detener();
    void actualizar(float deltaTiempo);
    bool agotado() const;
    float getTiempoRestante() const;
    bool estaActivo() const;

private:
    float tiempoRestante;
    bool activo;
};

#endif
