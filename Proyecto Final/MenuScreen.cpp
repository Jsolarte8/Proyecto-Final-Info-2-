#include "MenuScreen.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

static QPushButton* createGameButton(const QString& text)
{
    auto* button = new QPushButton(text);
    button->setCursor(Qt::PointingHandCursor);
    button->setMinimumHeight(54);
    button->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(9, 25, 22, 210);"
        "border: 2px solid #7bd88f;"
        "border-radius: 14px;"
        "color: white;"
        "font-size: 18px;"
        "font-weight: 800;"
        "padding: 12px 30px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(40, 96, 79, 230);"
        "border-color: #f3d36b;"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(24, 64, 55, 240);"
        "}");
    return button;
}

static QPixmap loadMenuPixmap(const QString& path, const QSize& size, const QColor& color, const QString& text)
{
    QPixmap pixmap(path);
    if (!pixmap.isNull()) {
        return pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    QPixmap fallback(size);
    fallback.fill(Qt::transparent);
    QPainter painter(&fallback);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(color);
    painter.setPen(QPen(QColor("#d7ffcc"), 3));
    painter.drawRoundedRect(fallback.rect().adjusted(6, 6, -6, -6), 28, 28);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(fallback.rect().adjusted(16, 16, -16, -16), Qt::AlignCenter | Qt::TextWordWrap, text);
    return fallback;
}

MenuScreen::MenuScreen(QWidget* parent)
    : QWidget(parent),
      currentCharacter(),
      characterImage(new QLabel(this)),
      characterName(new QLabel(this))
{
    setStyleSheet(
        "QWidget { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #061512, stop:0.46 #0d2a2f, stop:1 #14200f); color: white; }"
        "QLabel { background: transparent; }");

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(40, 30, 40, 30);
    root->setSpacing(12);

    auto* title = new QLabel("PÉRTIGA EN EL REINO MINIATURA", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 38px; font-weight: 900; color: #f7ffd7;");

    auto* subtitle = new QLabel("LA AVENTURA DE LINO", this);
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("font-size: 20px; font-weight: 700; color: #8fe9ff;");

    auto* ambience = new QLabel("✦ hojas altas   gotas brillantes   raíces antiguas   telarañas luminosas ✦", this);
    ambience->setAlignment(Qt::AlignCenter);
    ambience->setStyleSheet("font-size: 16px; color: #c7efb4;");

    root->addWidget(title);
    root->addWidget(subtitle);
    root->addWidget(ambience);

    auto* content = new QHBoxLayout();
    content->setSpacing(28);

    auto* leftPanel = new QVBoxLayout();
    characterImage->setAlignment(Qt::AlignCenter);
    characterName->setAlignment(Qt::AlignCenter);
    characterName->setStyleSheet("font-size: 22px; font-weight: 800; color: #e8ffd6;");
    leftPanel->addStretch();
    leftPanel->addWidget(characterImage);
    leftPanel->addWidget(characterName);
    leftPanel->addStretch();

    auto* buttonPanel = new QVBoxLayout();
    buttonPanel->setSpacing(14);
    auto* startButton = createGameButton("INICIAR AVENTURA");
    auto* characterButton = createGameButton("ELEGIR PERSONAJE");
    auto* difficultyButton = createGameButton("DIFICULTAD");
    auto* instructionsButton = createGameButton("INSTRUCCIONES");
    auto* exitButton = createGameButton("SALIR JUEGO");

    buttonPanel->addStretch();
    buttonPanel->addWidget(startButton);
    buttonPanel->addWidget(characterButton);
    buttonPanel->addWidget(difficultyButton);
    buttonPanel->addWidget(instructionsButton);
    buttonPanel->addWidget(exitButton);
    buttonPanel->addStretch();

    auto* spiderImage = new QLabel(this);
    spiderImage->setAlignment(Qt::AlignCenter);
    spiderImage->setPixmap(loadMenuPixmap(":/assets/arania_jefe.png", QSize(240, 260), QColor("#3c183a"), "Araña\nGuardiana"));
    auto* spiderName = new QLabel("ARAÑA GUARDIANA", this);
    spiderName->setAlignment(Qt::AlignCenter);
    spiderName->setStyleSheet("font-size: 22px; font-weight: 800; color: #ffd07d;");

    auto* rightPanel = new QVBoxLayout();
    rightPanel->addStretch();
    rightPanel->addWidget(spiderImage);
    rightPanel->addWidget(spiderName);
    rightPanel->addStretch();

    content->addLayout(leftPanel, 1);
    content->addLayout(buttonPanel, 1);
    content->addLayout(rightPanel, 1);
    root->addLayout(content, 1);

    connect(startButton, &QPushButton::clicked, this, &MenuScreen::startGameClicked);
    connect(characterButton, &QPushButton::clicked, this, &MenuScreen::charactersClicked);
    connect(difficultyButton, &QPushButton::clicked, this, &MenuScreen::difficultyClicked);
    connect(instructionsButton, &QPushButton::clicked, this, &MenuScreen::instructionsClicked);
    connect(exitButton, &QPushButton::clicked, this, &MenuScreen::exitClicked);

    updateCharacterPreview();
}

void MenuScreen::setCurrentCharacter(const Character& character)
{
    currentCharacter = character;
    updateCharacterPreview();
}

void MenuScreen::updateCharacterPreview()
{
    characterImage->setPixmap(loadMenuPixmap(currentCharacter.getLateralIdleSprite(),
                                             QSize(220, 250),
                                             QColor("#1a6b5e"),
                                             currentCharacter.getName()));
    characterName->setText("ATLETA: " + currentCharacter.getName());
}
