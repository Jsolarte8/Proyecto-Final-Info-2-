#ifndef CHARACTERSELECTSCREEN_H
#define CHARACTERSELECTSCREEN_H

#include "Character.h"

#include <QVector>
#include <QWidget>

class CharacterSelectScreen : public QWidget
{
    Q_OBJECT

public:
    explicit CharacterSelectScreen(QWidget* parent = nullptr);

signals:
    void characterSelected(Character character);
    void returnToMenu();

private:
    QVector<Character> characters;
};

#endif
