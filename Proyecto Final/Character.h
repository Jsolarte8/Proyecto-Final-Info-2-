#ifndef CHARACTER_H
#define CHARACTER_H

#include <QMetaType>
#include <QString>

class Character
{
public:
    Character();
    Character(const QString& name,
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
              const QString& cenitalChargeSprite);

    QString getName() const;
    QString getDescription() const;
    int getHealth() const;
    int getStrength() const;
    int getSpeed() const;
    int getJump() const;
    int getPoleControl() const;
    QString getLateralIdleSprite() const;
    QString getLateralWalk1Sprite() const;
    QString getLateralWalk2Sprite() const;
    QString getLateralJumpSprite() const;
    QString getLateralChargeSprite() const;
    QString getCenitalIdleSprite() const;
    QString getCenitalWalk1Sprite() const;
    QString getCenitalWalk2Sprite() const;
    QString getCenitalJumpSprite() const;
    QString getCenitalChargeSprite() const;

private:
    QString name;
    QString description;
    int health;
    int strength;
    int speed;
    int jump;
    int poleControl;
    QString lateralIdleSprite;
    QString lateralWalk1Sprite;
    QString lateralWalk2Sprite;
    QString lateralJumpSprite;
    QString lateralChargeSprite;
    QString cenitalIdleSprite;
    QString cenitalWalk1Sprite;
    QString cenitalWalk2Sprite;
    QString cenitalJumpSprite;
    QString cenitalChargeSprite;
};

Q_DECLARE_METATYPE(Character)

#endif
