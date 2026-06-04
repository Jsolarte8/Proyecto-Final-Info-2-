#include "CharacterSelectScreen.h"

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>

static QPushButton* createGameButton(const QString& text)
{
    auto* button = new QPushButton(text);
    button->setCursor(Qt::PointingHandCursor);
    button->setStyleSheet(
        "QPushButton { background-color: rgba(12, 32, 26, 220); border: 2px solid #85e89d; "
        "border-radius: 12px; color: white; font-size: 16px; font-weight: 800; padding: 10px 18px; }"
        "QPushButton:hover { background-color: rgba(42, 97, 76, 235); border-color: #ffd86b; }");
    return button;
}

static QPixmap loadCharacterPixmap(const QString& path, const QSize& size, const QColor& color, const QString& text)
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
    painter.setPen(QPen(QColor("#eaffd8"), 3));
    painter.drawRoundedRect(fallback.rect().adjusted(8, 8, -8, -8), 22, 22);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 17, QFont::Bold));
    painter.drawText(fallback.rect(), Qt::AlignCenter | Qt::TextWordWrap, text);
    return fallback;
}

static QLabel* statLabel(const QString& name, int value)
{
    auto* label = new QLabel(QString("%1: %2").arg(name).arg(value));
    label->setStyleSheet("font-size: 14px; color: #edf8dc;");
    return label;
}

CharacterSelectScreen::CharacterSelectScreen(QWidget* parent)
    : QWidget(parent)
{
    characters.append(Character("Lino",
                                "Atleta equilibrado del Reino Miniatura.",
                                100, 12, 14, 15, 15,
                                ":/assets/lino_lateral_idle.png",
                                ":/assets/lino_lateral_walk1.png",
                                ":/assets/lino_lateral_walk2.png",
                                ":/assets/lino_lateral_jump.png",
                                ":/assets/lino_lateral_charge.png",
                                ":/assets/lino_cenital_idle.png",
                                ":/assets/lino_cenital_walk1.png",
                                ":/assets/lino_cenital_walk2.png",
                                ":/assets/lino_cenital_jump.png",
                                ":/assets/lino_cenital_charge.png"));

    characters.append(Character("Nara",
                                "Exploradora veloz, experta en rutas de hojas y semillas.",
                                90, 10, 20, 14, 13,
                                ":/assets/nara_lateral_idle.png",
                                ":/assets/nara_lateral_walk1.png",
                                ":/assets/nara_lateral_walk2.png",
                                ":/assets/nara_lateral_jump.png",
                                ":/assets/nara_lateral_charge.png",
                                ":/assets/nara_cenital_idle.png",
                                ":/assets/nara_cenital_walk1.png",
                                ":/assets/nara_cenital_walk2.png",
                                ":/assets/nara_cenital_jump.png",
                                ":/assets/nara_cenital_charge.png"));

    characters.append(Character("Brumo",
                                "Guardián fuerte del jardín, más lento pero resistente.",
                                130, 17, 9, 11, 12,
                                ":/assets/brumo_lateral_idle.png",
                                ":/assets/brumo_lateral_walk1.png",
                                ":/assets/brumo_lateral_walk2.png",
                                ":/assets/brumo_lateral_jump.png",
                                ":/assets/brumo_lateral_charge.png",
                                ":/assets/brumo_cenital_idle.png",
                                ":/assets/brumo_cenital_walk1.png",
                                ":/assets/brumo_cenital_walk2.png",
                                ":/assets/brumo_cenital_jump.png",
                                ":/assets/brumo_cenital_charge.png"));

    setStyleSheet(
        "QWidget { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #081713, stop:1 #172813); color: white; }"
        "QLabel { background: transparent; }"
        "QProgressBar { border: 1px solid #5fcf8b; border-radius: 5px; background: #10241e; height: 9px; }"
        "QProgressBar::chunk { background: #f3d36b; border-radius: 5px; }");

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(36, 28, 36, 28);
    root->setSpacing(20);

    auto* title = new QLabel("ELIGE TU ATLETA", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 34px; font-weight: 900; color: #f7ffd7;");
    root->addWidget(title);

    auto* grid = new QGridLayout();
    grid->setSpacing(18);

    for (int i = 0; i < characters.size(); ++i) {
        const Character character = characters[i];
        auto* card = new QWidget(this);
        card->setStyleSheet("QWidget { background-color: rgba(8, 28, 24, 205); border: 2px solid #5fcf8b; border-radius: 16px; }"
                            "QLabel { border: none; }");

        auto* layout = new QVBoxLayout(card);
        layout->setContentsMargins(18, 18, 18, 18);
        layout->setSpacing(10);

        auto* image = new QLabel(card);
        image->setAlignment(Qt::AlignCenter);
        image->setPixmap(loadCharacterPixmap(character.getLateralIdleSprite(), QSize(170, 185), QColor("#22695a"), character.getName()));

        auto* name = new QLabel(character.getName(), card);
        name->setAlignment(Qt::AlignCenter);
        name->setStyleSheet("font-size: 24px; font-weight: 900; color: #f4ffd6;");

        auto* description = new QLabel(character.getDescription(), card);
        description->setWordWrap(true);
        description->setAlignment(Qt::AlignCenter);
        description->setStyleSheet("font-size: 14px; color: #d8f8df;");

        auto* stats = new QWidget(card);
        stats->setStyleSheet("QWidget { background: transparent; border: none; }");
        auto* statsLayout = new QVBoxLayout(stats);
        statsLayout->setContentsMargins(0, 0, 0, 0);
        statsLayout->addWidget(statLabel("Vida", character.getHealth()));
        statsLayout->addWidget(statLabel("Fuerza", character.getStrength()));
        statsLayout->addWidget(statLabel("Velocidad", character.getSpeed()));
        statsLayout->addWidget(statLabel("Salto", character.getJump()));
        statsLayout->addWidget(statLabel("Control de pértiga", character.getPoleControl()));

        auto* selectButton = createGameButton("SELECCIONAR");
        connect(selectButton, &QPushButton::clicked, this, [this, character]() {
            emit characterSelected(character);
        });

        layout->addWidget(image);
        layout->addWidget(name);
        layout->addWidget(description);
        layout->addWidget(stats);
        layout->addStretch();
        layout->addWidget(selectButton);

        grid->addWidget(card, 0, i);
    }

    root->addLayout(grid, 1);

    auto* backButton = createGameButton("VOLVER");
    connect(backButton, &QPushButton::clicked, this, &CharacterSelectScreen::returnToMenu);
    root->addWidget(backButton, 0, Qt::AlignCenter);
}
