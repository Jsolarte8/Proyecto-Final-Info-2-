#ifndef INTROSCREEN_H
#define INTROSCREEN_H

#include <QWidget>

class QLabel;
class QSequentialAnimationGroup;

class IntroScreen : public QWidget
{
    Q_OBJECT

public:
    explicit IntroScreen(QWidget* parent = nullptr);

signals:
    void introFinished();

protected:
    void showEvent(QShowEvent* event) override;

private:
    void startAnimation();

    QLabel* logoLabel;
    QSequentialAnimationGroup* animationGroup;
    bool started;
};

#endif
