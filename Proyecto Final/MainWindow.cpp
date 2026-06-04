#include "MainWindow.h"

#include "CharacterSelectScreen.h"
#include "DifficultyScreen.h"
#include "FinalBattleScreen.h"
#include "GameScreen.h"
#include "IntroScreen.h"
#include "MenuScreen.h"

#include <QColor>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

static QPushButton* createGameButton(const QString& text)
{
    auto* button = new QPushButton(text);
    button->setCursor(Qt::PointingHandCursor);
    button->setMinimumHeight(52);
    button->setStyleSheet(
        "QPushButton { background-color: rgba(12, 32, 26, 220); border: 2px solid #85e89d; "
        "border-radius: 12px; color: white; font-size: 17px; font-weight: 800; padding: 10px 24px; }"
        "QPushButton:hover { background-color: rgba(42, 97, 76, 235); border-color: #ffd86b; }");
    return button;
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      stackedWidget(new QStackedWidget(this)),
      introScreen(new IntroScreen(this)),
      menuScreen(new MenuScreen(this)),
      characterSelectScreen(new CharacterSelectScreen(this)),
      difficultyScreen(new DifficultyScreen(this)),
      gameScreen(new GameScreen(this)),
      finalBattleScreen(new FinalBattleScreen(this)),
      victoryScreen(nullptr),
      defeatScreen(nullptr),
      currentCharacter(),
      currentDifficulty(1)
{
    setWindowTitle("Pértiga en el Reino Miniatura");
    resize(1100, 720);
    setMinimumSize(960, 620);
    setCentralWidget(stackedWidget);

    victoryScreen = createEndScreen("VICTORIA FINAL",
                                    "HAS DERROTADO A LA ARAÑA GUARDIANA\nEl Reino Miniatura vuelve a respirar bajo las hojas.",
                                    QColor("#85e89d"));
    defeatScreen = createEndScreen("DERROTA",
                                   "LA ARAÑA TE ATRAPÓ\nVuelve al jardín, carga la pértiga y prueba otra ruta.",
                                   QColor("#ff6b7a"));

    stackedWidget->addWidget(introScreen);
    stackedWidget->addWidget(menuScreen);
    stackedWidget->addWidget(characterSelectScreen);
    stackedWidget->addWidget(difficultyScreen);
    stackedWidget->addWidget(gameScreen);
    stackedWidget->addWidget(finalBattleScreen);
    stackedWidget->addWidget(victoryScreen);
    stackedWidget->addWidget(defeatScreen);

    menuScreen->setCurrentCharacter(currentCharacter);

    connect(introScreen, &IntroScreen::introFinished, this, &MainWindow::showMenu);
    connect(menuScreen, &MenuScreen::startGameClicked, this, &MainWindow::startAdventure);
    connect(menuScreen, &MenuScreen::charactersClicked, this, [this]() {
        stackedWidget->setCurrentWidget(characterSelectScreen);
    });
    connect(menuScreen, &MenuScreen::difficultyClicked, this, [this]() {
        stackedWidget->setCurrentWidget(difficultyScreen);
    });
    connect(menuScreen, &MenuScreen::instructionsClicked, this, [this]() {
        QMessageBox::information(this,
                                 "Instrucciones",
                                 "Nivel 1:\nA/D mover, W salto corto, mantener Espacio carga pértiga, soltar Espacio salta con pértiga.\n\n"
                                 "Nivel 2:\nW/A/S/D mover, Espacio impulso corto.\n\n"
                                 "Batalla final:\nA/D mover, J golpe, K defensa, L especial, Espacio esquivar.\n\n"
                                 "R reinicia el nivel. ESC vuelve al menú.");
    });
    connect(menuScreen, &MenuScreen::exitClicked, this, &MainWindow::close);

    connect(characterSelectScreen, &CharacterSelectScreen::characterSelected, this, [this](Character character) {
        currentCharacter = character;
        menuScreen->setCurrentCharacter(currentCharacter);
        showMenu();
    });
    connect(characterSelectScreen, &CharacterSelectScreen::returnToMenu, this, &MainWindow::showMenu);

    connect(difficultyScreen, &DifficultyScreen::difficultySelected, this, [this](int difficulty) {
        currentDifficulty = difficulty;
        showMenu();
    });
    connect(difficultyScreen, &DifficultyScreen::returnToMenu, this, &MainWindow::showMenu);

    connect(gameScreen, &GameScreen::returnToMenu, this, &MainWindow::showMenu);
    connect(gameScreen, &GameScreen::levelTwoFinished, this, &MainWindow::showFinalBattle);
    connect(gameScreen, &GameScreen::gameLost, this, &MainWindow::showDefeat);

    connect(finalBattleScreen, &FinalBattleScreen::finalBattleWon, this, &MainWindow::showVictory);
    connect(finalBattleScreen, &FinalBattleScreen::finalBattleLost, this, &MainWindow::showDefeat);
    connect(finalBattleScreen, &FinalBattleScreen::returnToMenu, this, &MainWindow::showMenu);

    stackedWidget->setCurrentWidget(introScreen);
}

QWidget* MainWindow::createEndScreen(const QString& title, const QString& subtitle, const QColor& accentColor)
{
    auto* screen = new QWidget(this);
    screen->setStyleSheet(QString(
                              "QWidget { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #071512, stop:1 #1d1626); color: white; }"
                              "QLabel { background: transparent; }"));

    auto* layout = new QVBoxLayout(screen);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(24);
    layout->addStretch();

    auto* titleLabel = new QLabel(title, screen);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(QString("font-size: 42px; font-weight: 900; color: %1;").arg(accentColor.name()));

    auto* subtitleLabel = new QLabel(subtitle, screen);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setWordWrap(true);
    subtitleLabel->setStyleSheet("font-size: 20px; color: #f4ffd6;");

    auto* menuButton = createGameButton("VOLVER AL MENÚ");
    connect(menuButton, &QPushButton::clicked, this, &MainWindow::showMenu);

    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addWidget(menuButton, 0, Qt::AlignCenter);
    layout->addStretch();
    return screen;
}

void MainWindow::showMenu()
{
    stackedWidget->setCurrentWidget(menuScreen);
}

void MainWindow::startAdventure()
{
    gameScreen->setCharacter(currentCharacter);
    gameScreen->setDifficulty(currentDifficulty);
    stackedWidget->setCurrentWidget(gameScreen);
    gameScreen->startGame();
}

void MainWindow::showFinalBattle()
{
    finalBattleScreen->setPlayer(currentCharacter);
    finalBattleScreen->setDifficulty(currentDifficulty);
    stackedWidget->setCurrentWidget(finalBattleScreen);
    finalBattleScreen->startBattle();
}

void MainWindow::showVictory()
{
    stackedWidget->setCurrentWidget(victoryScreen);
}

void MainWindow::showDefeat()
{
    stackedWidget->setCurrentWidget(defeatScreen);
}
