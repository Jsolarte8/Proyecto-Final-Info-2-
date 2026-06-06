#ifndef FINALBATTLESCREEN_H
#define FINALBATTLESCREEN_H

#include "Character.h"

#include <QSet>
#include <QString>
#include <QWidget>

class QLabel;
class QGraphicsScene;
class QGraphicsView;
class QProgressBar;
class QTimer;

class FinalBattleScreen : public QWidget
{
    Q_OBJECT

public:
    explicit FinalBattleScreen(QWidget* parent = nullptr);

    void setPlayer(Character character);
    void setDifficulty(int difficulty);
    void startBattle();
    void resetBattle();

signals:
    void finalBattleWon();
    void finalBattleLost();
    void returnToMenu();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void updateBattle();
    void spiderAttack();

private:
    void renderBattle();
    void playerBasicAttack();
    void playerSpecialAttack();
    void playerDodge();
    void finishBattle(bool won);

    Character player;
    int difficulty;
    int playerHealth;
    int playerMaxHealth;
    int spiderHealth;
    int spiderMaxHealth;
    int spiderDamage;
    int spiderAttackInterval;
    float playerX;
    float spiderX;
    float specialCooldown;
    float dodgeCooldown;
    float hitFlash;
    float spiderAttackFlash;
    float playerAttackFlash;
    int battleTick;
    QString lastSpiderAction;
    QString playerAction;
    bool defending;
    bool battleEnded;
    int spiderAttackPattern;

    QGraphicsView* view;
    QGraphicsScene* scene;
    QProgressBar* playerHealthBar;
    QProgressBar* spiderHealthBar;
    QLabel* statusLabel;
    QLabel* cooldownLabel;
    QTimer* updateTimer;
    QTimer* spiderTimer;
    QSet<int> pressedKeys;
};

#endif
