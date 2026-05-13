#ifndef FISICA_H
#define FISICA_H

class Fisica {
public:
    virtual ~Fisica() {}

    virtual void calcular(float deltaTiempo) = 0;
};

#endif