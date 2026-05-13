#include "SistemaColisiones.h"

bool SistemaColisiones::verificarColision(Entidad* a, Entidad* b) {
    if (a == nullptr || b == nullptr) {
        return false;
    }

    if (!a->estaActivo() || !b->estaActivo()) {
        return false;
    }

    bool colisionX = a->getX() < b->getX() + b->getAncho() &&
                     a->getX() + a->getAncho() > b->getX();

    bool colisionY = a->getY() < b->getY() + b->getAlto() &&
                     a->getY() + a->getAlto() > b->getY();

    return colisionX && colisionY;
}