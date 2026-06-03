#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <QRectF>
#include <QtGlobal>

class Entidad
{
public:
    Entidad(float x = 0.0f, float y = 0.0f, float ancho = 0.0f, float alto = 0.0f)
        : x(x), y(y), ancho(ancho), alto(alto), activo(true)
    {
    }

    virtual ~Entidad() = default;

    virtual void actualizar(float deltaTiempo)
    {
        Q_UNUSED(deltaTiempo);
    }

    float getX() const { return x; }
    float getY() const { return y; }
    float getAncho() const { return ancho; }
    float getAlto() const { return alto; }
    bool estaActivo() const { return activo; }

    void setX(float value) { x = value; }
    void setY(float value) { y = value; }
    void setAncho(float value) { ancho = value; }
    void setAlto(float value) { alto = value; }
    void setActivo(bool value) { activo = value; }
    void setPosicion(float nuevoX, float nuevoY)
    {
        x = nuevoX;
        y = nuevoY;
    }

    QRectF rect() const { return QRectF(x, y, ancho, alto); }

protected:
    float x;
    float y;
    float ancho;
    float alto;
    bool activo;
};

#endif
