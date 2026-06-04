#include "Character.h"

Character::Character()
    : Character("Lino",
                "Atleta equilibrado del Reino Miniatura.",
                100,
                12,
                14,
                15,
                15,
                ":/assets/lino_lateral_idle.png",
                ":/assets/lino_lateral_walk1.png",
                ":/assets/lino_lateral_walk2.png",
                ":/assets/lino_lateral_jump.png",
                ":/assets/lino_lateral_charge.png",
                ":/assets/lino_cenital_idle.png",
                ":/assets/lino_cenital_walk1.png",
                ":/assets/lino_cenital_walk2.png",
                ":/assets/lino_cenital_jump.png",
                ":/assets/lino_cenital_charge.png")
{
}

Character::Character(const QString& name,
                     const QString& description,
                     int health,
                     int strength,
                     int speed,
                     int jump,
                     int poleControl,
                     const QString& lateralIdleSprite,
                     const QString& lateralWalk1Sprite,
                     const QString& lateralWalk2Sprite,
                     const QString& lateralJumpSprite,
                     const QString& lateralChargeSprite,
                     const QString& cenitalIdleSprite,
                     const QString& cenitalWalk1Sprite,
                     const QString& cenitalWalk2Sprite,
                     const QString& cenitalJumpSprite,
                     const QString& cenitalChargeSprite)
    : name(name),
      description(description),
      health(health),
      strength(strength),
      speed(speed),
      jump(jump),
      poleControl(poleControl),
      lateralIdleSprite(lateralIdleSprite),
      lateralWalk1Sprite(lateralWalk1Sprite),
      lateralWalk2Sprite(lateralWalk2Sprite),
      lateralJumpSprite(lateralJumpSprite),
      lateralChargeSprite(lateralChargeSprite),
      cenitalIdleSprite(cenitalIdleSprite),
      cenitalWalk1Sprite(cenitalWalk1Sprite),
      cenitalWalk2Sprite(cenitalWalk2Sprite),
      cenitalJumpSprite(cenitalJumpSprite),
      cenitalChargeSprite(cenitalChargeSprite)
{
}

QString Character::getName() const { return name; }
QString Character::getDescription() const { return description; }
int Character::getHealth() const { return health; }
int Character::getStrength() const { return strength; }
int Character::getSpeed() const { return speed; }
int Character::getJump() const { return jump; }
int Character::getPoleControl() const { return poleControl; }
QString Character::getLateralIdleSprite() const { return lateralIdleSprite; }
QString Character::getLateralWalk1Sprite() const { return lateralWalk1Sprite; }
QString Character::getLateralWalk2Sprite() const { return lateralWalk2Sprite; }
QString Character::getLateralJumpSprite() const { return lateralJumpSprite; }
QString Character::getLateralChargeSprite() const { return lateralChargeSprite; }
QString Character::getCenitalIdleSprite() const { return cenitalIdleSprite; }
QString Character::getCenitalWalk1Sprite() const { return cenitalWalk1Sprite; }
QString Character::getCenitalWalk2Sprite() const { return cenitalWalk2Sprite; }
QString Character::getCenitalJumpSprite() const { return cenitalJumpSprite; }
QString Character::getCenitalChargeSprite() const { return cenitalChargeSprite; }
