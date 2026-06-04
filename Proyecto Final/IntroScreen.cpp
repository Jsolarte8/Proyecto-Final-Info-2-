#include "IntroScreen.h"

#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QPauseAnimation>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QShowEvent>
#include <QVBoxLayout>

IntroScreen::IntroScreen(QWidget* parent)
    : QWidget(parent),
      logoLabel(new QLabel(this)),
      animationGroup(new QSequentialAnimationGroup(this)),
      started(false)
{
    setStyleSheet("QWidget { background-color: #07110d; color: white; }");

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(18);
    layout->addStretch();

    QPixmap logo(":/assets/logo.png");
    if (!logo.isNull()) {
        logoLabel->setPixmap(logo.scaled(520, 260, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        logoLabel->setText("PÉRTIGA EN EL REINO MINIATURA");
        logoLabel->setStyleSheet("font-size: 42px; font-weight: 900; color: #e6ffd2;");
    }
    logoLabel->setAlignment(Qt::AlignCenter);

    auto* subtitle = new QLabel("Momento III - Proyecto Final Informática II", this);
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("font-size: 20px; color: #b9f7d2;");

    layout->addWidget(logoLabel);
    layout->addWidget(subtitle);
    layout->addStretch();

    auto* effect = new QGraphicsOpacityEffect(logoLabel);
    effect->setOpacity(0.0);
    logoLabel->setGraphicsEffect(effect);

    auto* fadeIn = new QPropertyAnimation(effect, "opacity", this);
    fadeIn->setDuration(1500);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);

    auto* pause = new QPauseAnimation(1150, this);

    auto* fadeOut = new QPropertyAnimation(effect, "opacity", this);
    fadeOut->setDuration(1300);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);

    animationGroup->addAnimation(fadeIn);
    animationGroup->addAnimation(pause);
    animationGroup->addAnimation(fadeOut);

    connect(animationGroup, &QSequentialAnimationGroup::finished, this, &IntroScreen::introFinished);
}

void IntroScreen::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    if (!started) {
        started = true;
        startAnimation();
    }
}

void IntroScreen::startAnimation()
{
    animationGroup->start();
}
