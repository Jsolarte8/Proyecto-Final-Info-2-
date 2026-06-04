#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Character.h"

#include <QMainWindow>

class CharacterSelectScreen;
class DifficultyScreen;
class FinalBattleScreen;
class GameScreen;
class IntroScreen;
class MenuScreen;
class QStackedWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    QWidget* createEndScreen(const QString& title, const QString& subtitle, const QColor& accentColor);
    void showMenu();
    void startAdventure();
    void showFinalBattle();
    void showVictory();
    void showDefeat();

    QStackedWidget* stackedWidget;
    IntroScreen* introScreen;
    MenuScreen* menuScreen;
    CharacterSelectScreen* characterSelectScreen;
    DifficultyScreen* difficultyScreen;
    GameScreen* gameScreen;
    FinalBattleScreen* finalBattleScreen;
    QWidget* victoryScreen;
    QWidget* defeatScreen;
    Character currentCharacter;
    int currentDifficulty;
};

#endif
