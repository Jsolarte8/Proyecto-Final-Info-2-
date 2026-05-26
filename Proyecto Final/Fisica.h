#ifndef FISICA_H
#define FISICA_H

class Fisica
{
public:
    virtual ~Fisica() = default;
    virtual void calcular(float deltaTiempo) = 0;
};

#endif
