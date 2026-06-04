#include "DifficultyScreen.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

static QPushButton* createGameButton(const QString& text)
{
    auto* button = new QPushButton(text);
    button->setCursor(Qt::PointingHandCursor);
    button->setMinimumHeight(56);
    button->setStyleSheet(
        "QPushButton { background-color: rgba(12, 32, 26, 220); border: 2px solid #85e89d; "
        "border-radius: 12px; color: white; font-size: 17px; font-weight: 800; padding: 10px 22px; }"
        "QPushButton:hover { background-color: rgba(42, 97, 76, 235); border-color: #ffd86b; }");
    return button;
}

static QWidget* createDifficultyCard(const QString& title, const QString& body, int difficulty, DifficultyScreen* owner)
{
    auto* card = new QWidget(owner);
    card->setStyleSheet("QWidget { background-color: rgba(8, 28, 24, 210); border: 2px solid #5fcf8b; border-radius: 16px; }"
                        "QLabel { background: transparent; border: none; }");
    auto* layout = new QVBoxLayout(card);
    layout->setContentsMargins(22, 22, 22, 22);
    layout->setSpacing(14);

    auto* titleLabel = new QLabel(title, card);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 26px; font-weight: 900; color: #f7ffd7;");

    auto* bodyLabel = new QLabel(body, card);
    bodyLabel->setWordWrap(true);
    bodyLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    bodyLabel->setStyleSheet("font-size: 15px; line-height: 1.3; color: #d8f8df;");

    auto* button = createGameButton("ELEGIR");
    QObject::connect(button, &QPushButton::clicked, owner, [owner, difficulty]() {
        emit owner->difficultySelected(difficulty);
    });

    layout->addWidget(titleLabel);
    layout->addWidget(bodyLabel, 1);
    layout->addWidget(button);
    return card;
}

DifficultyScreen::DifficultyScreen(QWidget* parent)
    : QWidget(parent)
{
    setStyleSheet(
        "QWidget { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #071512, stop:1 #162412); color: white; }"
        "QLabel { background: transparent; }");

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(40, 32, 40, 32);
    root->setSpacing(22);

    auto* title = new QLabel("SELECCIONA DIFICULTAD", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 34px; font-weight: 900; color: #f7ffd7;");
    root->addWidget(title);

    auto* cards = new QHBoxLayout();
    cards->setSpacing(18);
    cards->addWidget(createDifficultyCard("FÁCIL",
                                          "Más tiempo en el Nivel 1.\nMenos gotas móviles.\nAraña más lenta.\nMenos hilos dinámicos.",
                                          0,
                                          this));
    cards->addWidget(createDifficultyCard("NORMAL",
                                          "Ritmo balanceado.\nTiempo justo.\nAraña atenta.\nObstáculos equilibrados.",
                                          1,
                                          this));
    cards->addWidget(createDifficultyCard("DIFÍCIL",
                                          "Menos tiempo.\nMás gotas móviles.\nAraña más rápida.\nMás hilos dinámicos.\nJefe final más fuerte.",
                                          2,
                                          this));

    root->addLayout(cards, 1);

    auto* backButton = createGameButton("VOLVER");
    connect(backButton, &QPushButton::clicked, this, &DifficultyScreen::returnToMenu);
    root->addWidget(backButton, 0, Qt::AlignCenter);
}
