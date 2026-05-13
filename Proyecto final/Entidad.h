#ifndef ENTIDAD_H
#define ENTIDAD_H

class Entidad {
protected:
    float x;
    float y;
    float ancho;
    float alto;
    bool activo;

public:
    Entidad(float x = 0, float y = 0, float ancho = 0, float alto = 0)
        : x(x), y(y), ancho(ancho), alto(alto), activo(true) {}

    virtual ~Entidad() {}

    virtual void actualizar(float deltaTiempo) = 0;

    float getX() const { return x; }
    float getY() const { return y; }
    float getAncho() const { return ancho; }
    float getAlto() const { return alto; }

    void setX(float nuevoX) { x = nuevoX; }
    void setY(float nuevoY) { y = nuevoY; }
    void setPosicion(float nuevoX, float nuevoY) {
        x = nuevoX;
        y = nuevoY;
    }

    bool estaActivo() const { return activo; }
    void activar() { activo = true; }
    void desactivar() { activo = false; }
};

#endif