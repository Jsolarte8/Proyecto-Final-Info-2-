#ifndef DIFFICULTYSCREEN_H
#define DIFFICULTYSCREEN_H

#include <QWidget>

class DifficultyScreen : public QWidget
{
    Q_OBJECT

public:
    explicit DifficultyScreen(QWidget* parent = nullptr);

signals:
    void difficultySelected(int difficulty);
    void returnToMenu();
};

#endif
