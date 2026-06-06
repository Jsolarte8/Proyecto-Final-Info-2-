#include "FinalBattleScreen.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QFrame>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QPixmapCache>
#include <QProgressBar>
#include <QPushButton>
#include <QSoundEffect>
#include <QTimer>
#include <QTransform>
#include <QUrl>
#include <QVBoxLayout>
#include <QtMath>

#include <stdexcept>

static QPushButton* createGameButton(const QString& text)
{
    auto* button = new QPushButton(text);
    button->setCursor(Qt::PointingHandCursor);
    button->setStyleSheet(
        "QPushButton { background-color: rgba(19, 18, 28, 230); border: 2px solid #ffd86b; "
        "border-radius: 11px; color: white; font-weight: 800; padding: 8px 16px; }"
        "QPushButton:hover { background-color: rgba(80, 55, 82, 235); border-color: #85e89d; }");
    return button;
}

static void ensureBattlePixmapCache()
{
    static bool configured = false;
    if (!configured) {
        QPixmapCache::setCacheLimit(131072);
        configured = true;
    }
}

static QPixmap battlePixmap(const QString& path, const QSize& requestedSize, const QColor& color, const QString& text)
{
    ensureBattlePixmapCache();
    const QSize size(qMax(8, requestedSize.width()), qMax(8, requestedSize.height()));
    const QString key = QString("battle-fit:%1:%2x%3:%4:%5")
                            .arg(path)
                            .arg(size.width())
                            .arg(size.height())
                            .arg(static_cast<quint32>(color.rgba()), 0, 16)
                            .arg(text);
    QPixmap cached;
    if (QPixmapCache::find(key, &cached)) {
        return cached;
    }

    QPixmap pixmap(path);
    if (!pixmap.isNull()) {
        QPixmap result = pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmapCache::insert(key, result);
        return result;
    }

    QPixmap fallback(size);
    fallback.fill(Qt::transparent);
    QPainter painter(&fallback);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(color);
    painter.setPen(QPen(color.lighter(160), 4));
    painter.drawRoundedRect(fallback.rect().adjusted(4, 4, -4, -4), 20, 20);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", qMax(12, qMin(size.width(), size.height()) / 6), QFont::Bold));
    painter.drawText(fallback.rect().adjusted(10, 10, -10, -10), Qt::AlignCenter | Qt::TextWordWrap, text);
    QPixmapCache::insert(key, fallback);
    return fallback;
}

static QPixmap filledBattlePixmap(const QString& path, const QSize& requestedSize, const QColor& color, const QString& text)
{
    ensureBattlePixmapCache();
    const QSize size(qMax(8, requestedSize.width()), qMax(8, requestedSize.height()));
    const QString key = QString("battle-fill:%1:%2x%3:%4:%5")
                            .arg(path)
                            .arg(size.width())
                            .arg(size.height())
                            .arg(static_cast<quint32>(color.rgba()), 0, 16)
                            .arg(text);
    QPixmap cached;
    if (QPixmapCache::find(key, &cached)) {
        return cached;
    }

    QPixmap pixmap(path);
    if (!pixmap.isNull()) {
        QPixmap result = pixmap.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QPixmapCache::insert(key, result);
        return result;
    }

    QPixmap fallback = battlePixmap(path, size, color, text);
    QPixmapCache::insert(key, fallback);
    return fallback;
}

static QPixmap mirrorBattlePixmap(const QPixmap& pixmap)
{
    ensureBattlePixmapCache();
    const QString key = QString("battle-mirror:%1").arg(pixmap.cacheKey());
    QPixmap cached;
    if (QPixmapCache::find(key, &cached)) {
        return cached;
    }

    QPixmap result = pixmap.transformed(QTransform().scale(-1.0, 1.0), Qt::SmoothTransformation);
    QPixmapCache::insert(key, result);
    return result;
}

FinalBattleScreen::FinalBattleScreen(QWidget* parent)
    : QWidget(parent),
      player(),
      difficulty(1),
      playerHealth(100),
      playerMaxHealth(100),
      spiderHealth(150),
      spiderMaxHealth(150),
      spiderDamage(12),
      spiderAttackInterval(1600),
      playerX(140.0f),
      spiderX(710.0f),
      specialCooldown(0.0f),
      dodgeCooldown(0.0f),
      hitFlash(0.0f),
      spiderAttackFlash(0.0f),
      playerAttackFlash(0.0f),
      battleTick(0),
      lastSpiderAction(""),
      playerAction("idle"),
      defending(false),
      battleEnded(false),
      spiderAttackPattern(0),
      view(new QGraphicsView(this)),
      scene(new QGraphicsScene(this)),
      playerHealthBar(new QProgressBar(this)),
      spiderHealthBar(new QProgressBar(this)),
      statusLabel(new QLabel(this)),
      cooldownLabel(new QLabel(this)),
      updateTimer(new QTimer(this)),
      spiderTimer(new QTimer(this)),
      hitSound(new QSoundEffect(this)),
      spiderSound(new QSoundEffect(this))
{
    setFocusPolicy(Qt::StrongFocus);
    setStyleSheet(
        "QWidget { background-color: #0a0710; color: white; }"
        "QLabel { background: transparent; font-weight: 800; color: #f8efd4; }"
        "QProgressBar { border: 2px solid #f3d36b; border-radius: 8px; background: #241929; text-align: center; color: white; height: 23px; }"
        "QProgressBar::chunk { background: #75d98f; border-radius: 6px; }");

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    auto* top = new QWidget(this);
    top->setStyleSheet("QWidget { background-color: rgba(8, 7, 14, 245); }");
    auto* topLayout = new QVBoxLayout(top);
    topLayout->setContentsMargins(18, 12, 18, 12);
    topLayout->setSpacing(8);

    auto* title = new QLabel("BATALLA FINAL: ARAÑA GUARDIANA", top);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 26px; font-weight: 900; color: #ffd86b;");

    auto* round = new QLabel("ROUND FINAL", top);
    round->setAlignment(Qt::AlignCenter);
    round->setStyleSheet("font-size: 16px; color: #8fe9ff;");

    auto* bars = new QHBoxLayout();
    playerHealthBar->setFormat(player.getName() + " %v");
    spiderHealthBar->setFormat("Araña %v");
    spiderHealthBar->setInvertedAppearance(true);
    bars->addWidget(playerHealthBar);
    bars->addWidget(spiderHealthBar);

    auto* bottomInfo = new QHBoxLayout();
    statusLabel->setText("J: golpe | K: defensa | L: especial | Espacio: esquivar");
    cooldownLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    bottomInfo->addWidget(statusLabel, 2);
    bottomInfo->addWidget(cooldownLabel, 1);

    topLayout->addWidget(title);
    topLayout->addWidget(round);
    topLayout->addLayout(bars);
    topLayout->addLayout(bottomInfo);

    view->setScene(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::SmoothPixmapTransform);
    view->setFrameShape(QFrame::NoFrame);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto* controls = new QWidget(this);
    controls->setStyleSheet("QWidget { background-color: rgba(8, 7, 14, 245); }");
    auto* controlsLayout = new QHBoxLayout(controls);
    controlsLayout->setContentsMargins(12, 8, 12, 8);
    auto* resetButton = createGameButton("REINICIAR BATALLA");
    auto* menuButton = createGameButton("MENÚ");
    controlsLayout->addStretch();
    controlsLayout->addWidget(resetButton);
    controlsLayout->addWidget(menuButton);

    root->addWidget(top);
    root->addWidget(view, 1);
    root->addWidget(controls);

    connect(resetButton, &QPushButton::clicked, this, &FinalBattleScreen::resetBattle);
    connect(menuButton, &QPushButton::clicked, this, [this]() {
        updateTimer->stop();
        spiderTimer->stop();
        emit returnToMenu();
    });
    connect(updateTimer, &QTimer::timeout, this, &FinalBattleScreen::updateBattle);
    connect(spiderTimer, &QTimer::timeout, this, &FinalBattleScreen::spiderAttack);

    hitSound->setSource(QUrl("qrc:/assets/sfx_golpe.wav"));
    hitSound->setVolume(0.58f);
    spiderSound->setSource(QUrl("qrc:/assets/sfx_danio.wav"));
    spiderSound->setVolume(0.50f);
}

void FinalBattleScreen::setPlayer(Character character)
{
    player = character;
}

void FinalBattleScreen::setDifficulty(int difficulty)
{
    try {
        if (difficulty < 0 || difficulty > 2) {
            throw std::invalid_argument("La dificultad debe estar entre 0 y 2.");
        }
        this->difficulty = difficulty;
    } catch (const std::invalid_argument&) {
        this->difficulty = 1;
    }
}

void FinalBattleScreen::startBattle()
{
    resetBattle();
    setFocus();
}

void FinalBattleScreen::resetBattle()
{
    pressedKeys.clear();
    defending = false;
    battleEnded = false;
    spiderAttackPattern = 0;
    playerMaxHealth = player.getHealth();
    playerHealth = playerMaxHealth;

    if (difficulty == 0) {
        spiderMaxHealth = 130;
        spiderDamage = 8;
        spiderAttackInterval = 2200;
    } else if (difficulty == 2) {
        spiderMaxHealth = 180;
        spiderDamage = 18;
        spiderAttackInterval = 1000;
    } else {
        spiderMaxHealth = 150;
        spiderDamage = 12;
        spiderAttackInterval = 1600;
    }

    spiderHealth = spiderMaxHealth;
    const float arenaWidth = qMax(960.0f, static_cast<float>(view->viewport()->width()));
    playerX = arenaWidth * 0.20f;
    spiderX = arenaWidth * 0.70f;
    specialCooldown = 0.0f;
    dodgeCooldown = 0.0f;
    hitFlash = 0.0f;
    spiderAttackFlash = 0.0f;
    playerAttackFlash = 0.0f;
    battleTick = 0;
    lastSpiderAction.clear();
    playerAction = "idle";

    playerHealthBar->setRange(0, playerMaxHealth);
    playerHealthBar->setValue(playerHealth);
    playerHealthBar->setFormat(player.getName() + " %v");
    spiderHealthBar->setRange(0, spiderMaxHealth);
    spiderHealthBar->setValue(spiderHealth);
    statusLabel->setText("J: golpe | K: defensa | L: especial | Espacio: esquivar");

    updateTimer->start(16);
    spiderTimer->start(spiderAttackInterval);
    renderBattle();
    setFocus();
}

void FinalBattleScreen::keyPressEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat()) {
        QWidget::keyPressEvent(event);
        return;
    }

    if (event->key() == Qt::Key_Escape) {
        updateTimer->stop();
        spiderTimer->stop();
        emit returnToMenu();
        return;
    }

    pressedKeys.insert(event->key());

    if (event->key() == Qt::Key_J) {
        playerBasicAttack();
    } else if (event->key() == Qt::Key_L) {
        playerSpecialAttack();
    } else if (event->key() == Qt::Key_Space) {
        playerDodge();
    } else if (event->key() == Qt::Key_K) {
        defending = true;
        playerAction = "defend";
        statusLabel->setText("Defensa activa: daño reducido");
    }
}

void FinalBattleScreen::keyReleaseEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat()) {
        QWidget::keyReleaseEvent(event);
        return;
    }
    pressedKeys.remove(event->key());
    if (event->key() == Qt::Key_K) {
        defending = false;
        if (playerAttackFlash <= 0.0f) {
            playerAction = "idle";
        }
    }
}

void FinalBattleScreen::updateBattle()
{
    if (battleEnded) {
        return;
    }

    constexpr float delta = 0.016f;
    ++battleTick;
    specialCooldown = qMax(0.0f, specialCooldown - delta);
    dodgeCooldown = qMax(0.0f, dodgeCooldown - delta);
    hitFlash = qMax(0.0f, hitFlash - delta);
    spiderAttackFlash = qMax(0.0f, spiderAttackFlash - delta);
    playerAttackFlash = qMax(0.0f, playerAttackFlash - delta);

    const float speed = 260.0f + player.getSpeed() * 4.0f;
    bool moving = false;
    if (pressedKeys.contains(Qt::Key_A)) {
        playerX -= speed * delta;
        moving = true;
    }
    if (pressedKeys.contains(Qt::Key_D)) {
        playerX += speed * delta;
        moving = true;
    }

    if (!defending && playerAttackFlash <= 0.0f) {
        playerAction = moving ? "walk" : "idle";
    }

    const float arenaWidth = qMax(960.0f, static_cast<float>(view->viewport()->width()));
    playerX = qBound(60.0f, playerX, arenaWidth * 0.55f);
    if (spiderX - playerX > 210.0f) {
        spiderX -= (difficulty == 2 ? 86.0f : 60.0f) * delta;
    } else if (spiderX - playerX < 135.0f) {
        spiderX += 44.0f * delta;
    }
    spiderX = qBound(arenaWidth * 0.50f, spiderX, arenaWidth - 250.0f);

    playerHealthBar->setValue(playerHealth);
    spiderHealthBar->setValue(spiderHealth);
    cooldownLabel->setText(QString("Especial: %1 s").arg(specialCooldown, 0, 'f', 1));

    if (spiderHealth <= 0) {
        finishBattle(true);
        return;
    }
    if (playerHealth <= 0) {
        finishBattle(false);
        return;
    }

    renderBattle();
}

void FinalBattleScreen::spiderAttack()
{
    if (battleEnded) {
        return;
    }

    const float distancia = qAbs(spiderX - playerX);
    const int pattern = spiderAttackPattern++ % 3;
    int damage = spiderDamage;
    QString text;

    if (pattern == 0) {
        text = "Mordida rápida";
        if (distancia > 165.0f) {
            damage = 0;
        }
    } else if (pattern == 1) {
        text = "Lanzar hilo";
        damage = distancia < 330.0f ? spiderDamage / 2 + 3 : 0;
    } else {
        text = "Salto hacia el jugador";
        spiderX = qMax(560.0f, playerX + 145.0f);
        damage = distancia < 245.0f ? spiderDamage + 2 : spiderDamage / 2;
    }

    lastSpiderAction = text;
    spiderAttackFlash = 0.40f;
    if (pattern == 0) {
        spiderX = qMax(playerX + 135.0f, spiderX - 34.0f);
    }

    if (damage > 0) {
        if (defending) {
            damage = qMax(1, damage / 3);
            statusLabel->setText(text + " bloqueada parcialmente");
        } else {
            statusLabel->setText(text + " impacta");
        }
        playerHealth = qMax(0, playerHealth - damage);
        spiderSound->play();
        hitFlash = 0.18f;
    } else {
        statusLabel->setText(text + " falla");
    }
}

void FinalBattleScreen::renderBattle()
{
    scene->clear();

    const qreal sceneWidth = qMax<qreal>(960.0, view->viewport()->width());
    const qreal sceneHeight = qMax<qreal>(560.0, view->viewport()->height());
    scene->setSceneRect(0, 0, sceneWidth, sceneHeight);

    auto* background = scene->addPixmap(filledBattlePixmap(":/assets/fondo_nivel2.png",
                                                           QSize(static_cast<int>(sceneWidth), static_cast<int>(sceneHeight)),
                                                           QColor("#171125"),
                                                           "Telarana final"));
    background->setZValue(-10);

    const qreal groundY = sceneHeight - 120.0;
    auto* ground = scene->addRect(0, groundY, sceneWidth, 120,
                                  QPen(QColor(86, 111, 75, 180), 2),
                                  QBrush(QColor(18, 38, 21, 210)));
    ground->setZValue(-2);

    auto* title = scene->addText("BATALLA FINAL", QFont("Arial", 28, QFont::Bold));
    title->setDefaultTextColor(QColor("#ffd86b"));
    title->setPos((sceneWidth - title->boundingRect().width()) / 2.0, 24);

    QString playerSprite = player.getLateralIdleSprite();
    if (playerAction == "walk") {
        playerSprite = (battleTick / 10) % 2 == 0 ? player.getLateralWalk1Sprite() : player.getLateralWalk2Sprite();
    } else if (playerAction == "basic" || playerAction == "special" || defending) {
        playerSprite = player.getLateralChargeSprite();
    } else if (playerAction == "dodge") {
        playerSprite = player.getLateralJumpSprite();
    }

    auto* playerItem = scene->addPixmap(battlePixmap(playerSprite, QSize(155, 195), QColor("#1a7d69"), player.getName()));
    playerItem->setPos(playerX, groundY - 185.0);
    playerItem->setZValue(5);

    const bool walkingSpider = qAbs(spiderX - playerX) > 145.0f && spiderAttackFlash <= 0.0f;
    QString spiderSprite = ":/assets/arania_jefe.png";
    if (walkingSpider) {
        spiderSprite = (battleTick / 9) % 2 == 0 ? ":/assets/arania_walk1.png" : ":/assets/arania_walk2.png";
    }

    QPixmap spiderPixmap = battlePixmap(spiderSprite, QSize(245, 205), QColor("#3d173b"), "Arania");
    if (spiderX > playerX) {
        spiderPixmap = mirrorBattlePixmap(spiderPixmap);
    }
    auto* spiderItem = scene->addPixmap(spiderPixmap);
    const qreal spiderBob = walkingSpider ? qSin(battleTick * 0.35) * 5.0 : 0.0;
    spiderItem->setPos(spiderX, groundY - 205.0 + spiderBob);
    spiderItem->setZValue(5);

    auto* pole = scene->addPixmap(battlePixmap(":/assets/pertiga.png", QSize(190, 18), QColor("#85c76b"), ""));
    pole->setPos(playerX + 95, groundY - 118.0);
    pole->setRotation(playerAction == "special" ? -22 : -8);
    pole->setZValue(6);

    if (playerAttackFlash > 0.0f) {
        QPixmap strikePixmap = battlePixmap(":/assets/efecto_golpe.png",
                                            playerAction == "special" ? QSize(330, 195) : QSize(250, 145),
                                            QColor("#ffe46b"),
                                            "");
        const bool spiderAtRight = spiderX >= playerX;
        if (!spiderAtRight) {
            strikePixmap = mirrorBattlePixmap(strikePixmap);
        }

        auto* strike = scene->addPixmap(strikePixmap);
        strike->setPos(spiderAtRight ? playerX + 112.0 : playerX - strikePixmap.width() + 42.0,
                       playerAction == "special" ? groundY - 235.0 : groundY - 195.0);
        strike->setZValue(8);
    }

    if (spiderAttackFlash > 0.0f) {
        if (lastSpiderAction.contains("hilo", Qt::CaseInsensitive)) {
            const qreal startX = spiderX > playerX ? playerX + 110.0 : spiderX + 95.0;
            const qreal endX = spiderX > playerX ? spiderX + 55.0 : playerX + 125.0;
            auto* thread = scene->addPixmap(filledBattlePixmap(":/assets/efecto_hilo_ataque.png",
                                                               QSize(static_cast<int>(qAbs(endX - startX)), 58),
                                                               QColor("#dcefed"),
                                                               ""));
            thread->setPos(qMin(startX, endX), groundY - 166.0);
            thread->setZValue(8);
        } else {
            QPixmap bitePixmap = battlePixmap(":/assets/efecto_mordida.png",
                                              QSize(160, 92),
                                              QColor("#ff5e6b"),
                                              "");
            const bool spiderAtRight = spiderX >= playerX;
            if (!spiderAtRight) {
                bitePixmap = mirrorBattlePixmap(bitePixmap);
            }

            auto* bite = scene->addPixmap(bitePixmap);
            bite->setPos(spiderAtRight ? playerX + 82.0 : playerX - 80.0, groundY - 190.0);
            bite->setZValue(8);
        }

        auto* actionText = scene->addText(lastSpiderAction, QFont("Arial", 18, QFont::Bold));
        actionText->setDefaultTextColor(QColor("#ffdf7d"));
        actionText->setPos(spiderX - 20, groundY - 250);
        actionText->setZValue(9);
    }

    if (hitFlash > 0.0f) {
        auto* flash = scene->addText("GOLPE", QFont("Arial", 30, QFont::Bold));
        flash->setDefaultTextColor(QColor("#ff5e6b"));
        flash->setPos((sceneWidth - flash->boundingRect().width()) / 2.0, groundY - 280);
        flash->setZValue(10);
    }

    auto* controls = scene->addText("A/D mover    J golpe    K defensa    L especial    Espacio esquivar",
                                    QFont("Arial", 17, QFont::Bold));
    controls->setDefaultTextColor(QColor("#f7ffd7"));
    controls->setPos((sceneWidth - controls->boundingRect().width()) / 2.0, sceneHeight - 58);
    controls->setZValue(20);
}
void FinalBattleScreen::playerBasicAttack()
{
    if (battleEnded) {
        return;
    }

    playerAction = "basic";
    playerAttackFlash = 0.22f;

    const float distancia = qAbs(spiderX - playerX);
    if (distancia <= 230.0f) {
        const int damage = player.getStrength();
        spiderHealth = qMax(0, spiderHealth - damage);
        hitSound->play();
        statusLabel->setText(QString("Golpe básico: %1 de daño").arg(damage));
        hitFlash = 0.12f;
    } else {
        statusLabel->setText("Golpe básico fuera de alcance");
    }
}

void FinalBattleScreen::playerSpecialAttack()
{
    if (battleEnded || specialCooldown > 0.0f) {
        return;
    }

    playerAction = "special";
    playerAttackFlash = 0.34f;

    const float distancia = qAbs(spiderX - playerX);
    if (distancia <= 340.0f) {
        const int damage = player.getStrength() * 2 + player.getPoleControl();
        spiderHealth = qMax(0, spiderHealth - damage);
        hitSound->play();
        statusLabel->setText(QString("Ataque especial con pértiga: %1 de daño").arg(damage));
        specialCooldown = difficulty == 2 ? 4.2f : 3.4f;
        hitFlash = 0.2f;
    } else {
        statusLabel->setText("La pértiga no alcanza a la Araña");
        specialCooldown = 1.0f;
    }
}

void FinalBattleScreen::playerDodge()
{
    if (battleEnded || dodgeCooldown > 0.0f) {
        return;
    }

    playerAction = "dodge";
    playerAttackFlash = 0.28f;
    playerX = qMax(40.0f, playerX - 115.0f);
    dodgeCooldown = 1.2f;
    statusLabel->setText("Esquiva corta");
}

void FinalBattleScreen::finishBattle(bool won)
{
    battleEnded = true;
    updateTimer->stop();
    spiderTimer->stop();
    statusLabel->setText(won ? "HAS DERROTADO A LA ARAÑA GUARDIANA" : "DERROTA - LA ARAÑA TE ATRAPÓ");
    renderBattle();

    QTimer::singleShot(900, this, [this, won]() {
        if (won) {
            emit finalBattleWon();
        } else {
            emit finalBattleLost();
        }
    });
}
