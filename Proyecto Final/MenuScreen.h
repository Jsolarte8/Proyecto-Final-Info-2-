#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include "Character.h"

#include <QWidget>

class QLabel;

class MenuScreen : public QWidget
{
    Q_OBJECT

public:
    explicit MenuScreen(QWidget* parent = nullptr);
    void setCurrentCharacter(const Character& character);

signals:
    void startGameClicked();
    void charactersClicked();
    void difficultyClicked();
    void instructionsClicked();
    void exitClicked();

private:
    void updateCharacterPreview();

    Character currentCharacter;
    QLabel* characterImage;
    QLabel* characterName;
};

#endif
