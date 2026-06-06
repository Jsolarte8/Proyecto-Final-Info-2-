#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "Character.h"
#include "Juego.h"

#include <QSet>
#include <QWidget>

class QLabel;
class QGraphicsScene;
class QGraphicsView;
class QProgressBar;
class QSoundEffect;
class QTimer;

class GameScreen : public QWidget
{
    Q_OBJECT

public:
    explicit GameScreen(QWidget* parent = nullptr);
    ~GameScreen() override;

    void setCharacter(Character character);
    void setDifficulty(int difficulty);
    void startGame();
    void resetGame();
    void buildLevelScene();
    void updateHUD();

signals:
    void returnToMenu();
    void levelTwoFinished();
    void gameLost();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void updateGame();

private:
    QString playerSpritePath() const;
    void setupAudio();
    void startAmbientAudio();
    void stopAmbientAudio();
    void resetAudioTracking();
    void updateAudioFeedback();

    Character character;
    int difficulty;
    Juego* juego;
    QGraphicsView* view;
    QGraphicsScene* scene;
    QTimer* updateTimer;
    QLabel* levelLabel;
    QLabel* scoreLabel;
    QLabel* timeLabel;
    QLabel* collectiblesLabel;
    QProgressBar* healthBar;
    QProgressBar* poleBar;
    QSoundEffect* ambientSound;
    QSoundEffect* collectSound;
    QSoundEffect* damageSound;
    QSet<int> pressedKeys;
    bool spaceCharging;
    bool impulseRequested;
    int animationTick;
    int lastTrackedHealth;
    int lastTrackedNectar;
    int lastTrackedSemillas;
};

#endif
