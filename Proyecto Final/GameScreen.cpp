#include "GameScreen.h"
#include "NivelDos.h"
#include "NivelUno.h"

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
#include <QScrollBar>
#include <QSoundEffect>
#include <QTimer>
#include <QTransform>
#include <QUrl>
#include <QVBoxLayout>

#include <exception>

static QPushButton* createGameButton(const QString& text)
{
    auto* button = new QPushButton(text);
    button->setCursor(Qt::PointingHandCursor);
    button->setStyleSheet(
        "QPushButton { background-color: rgba(10, 30, 26, 220); border: 2px solid #75d98f; "
        "border-radius: 10px; color: white; font-weight: 800; padding: 7px 14px; }"
        "QPushButton:hover { background-color: rgba(42, 97, 76, 235); border-color: #ffd86b; }");
    return button;
}

static void ensureGamePixmapCache()
{
    static bool configured = false;
    if (!configured) {
        QPixmapCache::setCacheLimit(131072);
        configured = true;
    }
}

static QPixmap makeGamePixmap(const QString& path, const QSize& requestedSize, const QColor& color, const QString& text)
{
    ensureGamePixmapCache();
    const QSize size(qMax(8, requestedSize.width()), qMax(8, requestedSize.height()));
    const QString key = QString("fit:%1:%2x%3:%4:%5")
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
    painter.setPen(QPen(color.lighter(150), 2));
    painter.drawRoundedRect(fallback.rect().adjusted(1, 1, -2, -2), 8, 8);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", qMax(8, qMin(size.width(), size.height()) / 5), QFont::Bold));
    painter.drawText(fallback.rect().adjusted(4, 4, -4, -4), Qt::AlignCenter | Qt::TextWordWrap, text);
    QPixmapCache::insert(key, fallback);
    return fallback;
}

static QPixmap makeFilledGamePixmap(const QString& path, const QSize& requestedSize, const QColor& color, const QString& text)
{
    ensureGamePixmapCache();
    const QSize size(qMax(8, requestedSize.width()), qMax(8, requestedSize.height()));
    const QString key = QString("fill:%1:%2x%3:%4:%5")
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

    QPixmap fallback = makeGamePixmap(path, size, color, text);
    QPixmapCache::insert(key, fallback);
    return fallback;
}

static QPixmap mirroredPixmap(const QPixmap& pixmap)
{
    ensureGamePixmapCache();
    const QString key = QString("mirror:%1").arg(pixmap.cacheKey());
    QPixmap cached;
    if (QPixmapCache::find(key, &cached)) {
        return cached;
    }

    QPixmap result = pixmap.transformed(QTransform().scale(-1.0, 1.0), Qt::SmoothTransformation);
    QPixmapCache::insert(key, result);
    return result;
}

static QString obstacleResource(const QString& tipo)
{
    if (tipo == "piedra") return ":/assets/piedra.png";
    if (tipo == "raiz") return ":/assets/raiz.png";
    if (tipo == "charco") return ":/assets/charco.png";
    if (tipo == "gota") return ":/assets/gota.png";
    if (tipo == "hilo") return ":/assets/hilo.png";
    return ":/assets/piedra.png";
}

static QString collectableResource(const QString& tipo)
{
    if (tipo == "semilla") return ":/assets/semilla.png";
    return ":/assets/nectar.png";
}

static QColor obstacleColor(const QString& tipo)
{
    if (tipo == "piedra") return QColor("#5f6670");
    if (tipo == "raiz") return QColor("#7c4d2d");
    if (tipo == "charco") return QColor("#285d78");
    if (tipo == "gota") return QColor("#4bb8ff");
    if (tipo == "hilo") return QColor("#d7e8e6");
    return QColor("#607070");
}

GameScreen::GameScreen(QWidget* parent)
    : QWidget(parent),
      character(),
      difficulty(1),
      juego(new Juego()),
      view(new QGraphicsView(this)),
      scene(new QGraphicsScene(this)),
      updateTimer(new QTimer(this)),
      levelLabel(new QLabel(this)),
      scoreLabel(new QLabel(this)),
      timeLabel(new QLabel(this)),
      collectiblesLabel(new QLabel(this)),
      healthBar(new QProgressBar(this)),
      poleBar(new QProgressBar(this)),
      ambientSound(new QSoundEffect(this)),
      collectSound(new QSoundEffect(this)),
      damageSound(new QSoundEffect(this)),
      spaceCharging(false),
      impulseRequested(false),
      animationTick(0),
      lastTrackedHealth(0),
      lastTrackedNectar(0),
      lastTrackedSemillas(0)
{
    setFocusPolicy(Qt::StrongFocus);
    setStyleSheet(
        "QWidget { background-color: #071512; color: white; }"
        "QLabel { font-size: 14px; font-weight: 700; color: #ecffe0; }"
        "QProgressBar { border: 1px solid #67d68f; border-radius: 6px; background: #11241f; text-align: center; color: white; }"
        "QProgressBar::chunk { background: #72e095; border-radius: 5px; }");

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    auto* hud = new QWidget(this);
    hud->setStyleSheet("QWidget { background-color: rgba(5, 19, 16, 240); }");
    auto* hudLayout = new QHBoxLayout(hud);
    hudLayout->setContentsMargins(14, 9, 14, 9);
    hudLayout->setSpacing(12);

    levelLabel->setMinimumWidth(230);
    scoreLabel->setMinimumWidth(120);
    timeLabel->setMinimumWidth(100);
    collectiblesLabel->setMinimumWidth(150);

    healthBar->setRange(0, character.getHealth());
    healthBar->setValue(character.getHealth());
    healthBar->setFormat("Vida %v");
    healthBar->setFixedWidth(170);

    poleBar->setRange(0, 100);
    poleBar->setFormat("Pértiga %p%");
    poleBar->setFixedWidth(160);
    poleBar->setStyleSheet(
        "QProgressBar { border: 1px solid #ffd86b; border-radius: 6px; background: #242012; text-align: center; color: white; }"
        "QProgressBar::chunk { background: #ffd86b; border-radius: 5px; }");

    auto* menuButton = createGameButton("MENÚ");
    auto* resetButton = createGameButton("R");
    connect(menuButton, &QPushButton::clicked, this, [this]() {
        updateTimer->stop();
        stopAmbientAudio();
        emit returnToMenu();
    });
    connect(resetButton, &QPushButton::clicked, this, &GameScreen::resetGame);

    hudLayout->addWidget(levelLabel);
    hudLayout->addWidget(scoreLabel);
    hudLayout->addWidget(timeLabel);
    hudLayout->addWidget(collectiblesLabel);
    hudLayout->addWidget(healthBar);
    hudLayout->addWidget(poleBar);
    hudLayout->addStretch();
    hudLayout->addWidget(resetButton);
    hudLayout->addWidget(menuButton);

    view->setScene(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::SmoothPixmapTransform);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFrameShape(QFrame::NoFrame);

    root->addWidget(hud);
    root->addWidget(view, 1);

    updateTimer->setInterval(16);
    connect(updateTimer, &QTimer::timeout, this, &GameScreen::updateGame);
    setupAudio();
}

GameScreen::~GameScreen()
{
    stopAmbientAudio();
    delete juego;
    juego = nullptr;
}

void GameScreen::setCharacter(Character character)
{
    this->character = character;
    juego->setCharacter(character);
    healthBar->setRange(0, character.getHealth());
}

void GameScreen::setDifficulty(int difficulty)
{
    try {
        juego->setDifficulty(difficulty);
        this->difficulty = difficulty;
    } catch (const std::exception&) {
        this->difficulty = 1;
        juego->setDifficulty(1);
    }
}

void GameScreen::startGame()
{
    pressedKeys.clear();
    spaceCharging = false;
    impulseRequested = false;
    animationTick = 0;
    juego->setCharacter(character);
    juego->setDifficulty(difficulty);
    juego->iniciar();
    resetAudioTracking();
    startAmbientAudio();
    buildLevelScene();
    updateHUD();
    updateTimer->start();
    setFocus();
}

void GameScreen::resetGame()
{
    pressedKeys.clear();
    spaceCharging = false;
    impulseRequested = false;
    juego->reiniciarNivel();
    resetAudioTracking();
    startAmbientAudio();
    buildLevelScene();
    updateHUD();
    updateTimer->start();
    setFocus();
}

void GameScreen::buildLevelScene()
{
    scene->clear();
    Nivel* nivel = juego->getNivelActual();
    if (!nivel || !nivel->getJugador()) {
        return;
    }

    const bool nivelUno = juego->getNumeroNivel() == 1;
    const QSize viewportSize = view->viewport() ? view->viewport()->size() : QSize(1280, 720);
    const qreal sceneHeight = nivelUno
                                  ? qMax<qreal>(820.0, viewportSize.height())
                                  : qMax<qreal>(980.0, viewportSize.height());
    const qreal sceneWidth = nivelUno ? 2400.0 : qMax<qreal>(1650.0, viewportSize.width());
    const QRectF sceneRect(0, 0, sceneWidth, sceneHeight);
    scene->setSceneRect(sceneRect);

    const QString backgroundPath = nivelUno ? ":/assets/fondo_nivel1.png" : ":/assets/fondo_nivel2.png";
    const QColor backgroundColor = nivelUno ? QColor("#234f31") : QColor("#172331");
    auto* background = scene->addPixmap(makeFilledGamePixmap(backgroundPath, sceneRect.size().toSize(), backgroundColor, ""));
    background->setPos(0, 0);
    background->setZValue(-10);

    for (Plataforma* plataforma : nivel->getPlataformas()) {
        if (!nivelUno) {
            continue;
        }

        const QRectF rect = plataforma->rect();
        const bool suelo = plataforma->getTipo() == "hoja";
        const QString sprite = suelo ? ":/assets/plataforma_suelo.png" : ":/assets/plataforma_hoja.png";
        const qreal visualHeight = suelo ? 132.0 : 76.0;
        const qreal visualY = suelo ? rect.top() - 14.0 : rect.top() - 16.0;

        auto* item = scene->addPixmap(makeFilledGamePixmap(sprite,
                                                           QSize(static_cast<int>(rect.width()), static_cast<int>(visualHeight)),
                                                           QColor("#5f9d3d"),
                                                           suelo ? "suelo" : "hoja"));
        item->setPos(rect.left(), visualY);
        item->setZValue(suelo ? -1 : 1);
    }

    for (Obstaculo* obstaculo : nivel->getObstaculos()) {
        if (!obstaculo->estaActivo()) {
            continue;
        }

        const QRectF rect = obstaculo->rect();
        const QString tipo = obstaculo->getTipo();
        const bool esHilo = tipo == "hilo";
        const QString resource = esHilo
                                     ? (obstaculo->getAlto() > obstaculo->getAncho() ? ":/assets/hilo_vertical.png" : ":/assets/hilo.png")
                                     : obstacleResource(tipo);

        QSize visualSize = rect.size().toSize();
        QPointF visualPos = rect.topLeft();
        if (!esHilo) {
            if (tipo == "charco") {
                visualSize = QSize(static_cast<int>(rect.width() * 1.12), static_cast<int>(rect.height() * 2.2));
                visualPos = QPointF(rect.center().x() - visualSize.width() / 2.0, rect.bottom() - visualSize.height() + 7.0);

                auto* brillo = scene->addEllipse(rect.adjusted(-18, -18, 18, 12),
                                                 QPen(QColor(94, 225, 255, 175), 2),
                                                 QBrush(QColor(57, 164, 239, 62)));
                brillo->setZValue(0.2);
            } else if (tipo == "piedra" || tipo == "raiz" || tipo == "gota") {
                visualSize = QSize(static_cast<int>(rect.width() * 1.18), static_cast<int>(rect.height() * 1.18));
                visualPos = QPointF(rect.center().x() - visualSize.width() / 2.0, rect.center().y() - visualSize.height() / 2.0);

                if (tipo != "gota") {
                    auto* sombra = scene->addEllipse(QRectF(rect.left() - 6.0,
                                                            rect.bottom() - 10.0,
                                                            rect.width() + 12.0,
                                                            18.0),
                                                     Qt::NoPen,
                                                     QBrush(QColor(9, 21, 15, 110)));
                    sombra->setZValue(0.25);
                }
            }
        }

        const QPixmap pixmap = esHilo || tipo == "charco"
                                   ? makeFilledGamePixmap(resource, visualSize, obstacleColor(tipo), tipo)
                                   : makeGamePixmap(resource, visualSize, obstacleColor(tipo), tipo);

        auto* item = scene->addPixmap(pixmap);
        item->setPos(visualPos);
        item->setZValue(esHilo ? 2 : 1);
    }

    for (Recolectable* recolectable : nivel->getRecolectables()) {
        if (!recolectable->estaActivo()) {
            continue;
        }
        auto* item = scene->addPixmap(makeGamePixmap(collectableResource(recolectable->getTipo()),
                                                     recolectable->rect().size().toSize(),
                                                     recolectable->getTipo() == "semilla" ? QColor("#ffe46b") : QColor("#ffbf54"),
                                                     recolectable->getTipo()));
        item->setPos(recolectable->getX(), recolectable->getY());
        item->setZValue(4);
    }

    if (auto* nivel1 = dynamic_cast<NivelUno*>(nivel)) {
        auto* baseMeta = scene->addEllipse(QRectF(nivel1->getMetaRect().left() + 10.0,
                                                  nivel1->getSueloY() - 8.0,
                                                  nivel1->getMetaRect().width() - 16.0,
                                                  18.0),
                                           Qt::NoPen,
                                           QBrush(QColor(12, 40, 24, 135)));
        baseMeta->setZValue(4);

        const QPixmap metaPixmap = makeGamePixmap(":/assets/meta.png",
                                                  QSize(124, 150),
                                                  QColor("#6ecc80"),
                                                  "META");
        auto* meta = scene->addPixmap(metaPixmap);
        meta->setPos(nivel1->getMetaRect().left(),
                     nivel1->getSueloY() - metaPixmap.height() + 8.0);
        meta->setZValue(5);
    }

    if (auto* nivel2 = dynamic_cast<NivelDos*>(nivel)) {
        for (const QRectF& hilo : nivel2->getHilosDinamicos()) {
            const QString hiloSprite = hilo.height() > hilo.width() ? ":/assets/hilo_vertical.png" : ":/assets/hilo.png";
            auto* item = scene->addPixmap(makeFilledGamePixmap(hiloSprite,
                                                               hilo.size().toSize(),
                                                               QColor("#dcefed"),
                                                               "hilo"));
            item->setPos(hilo.topLeft());
            item->setZValue(3);
        }

        auto* salida = scene->addPixmap(makeGamePixmap(nivel2->salidaEstaAbierta() ? ":/assets/salida.png" : ":/assets/telarana_redonda.png",
                                                       nivel2->getSalidaRect().size().toSize(),
                                                       nivel2->salidaEstaAbierta() ? QColor("#68d879") : QColor("#b8c7c4"),
                                                       nivel2->salidaEstaAbierta() ? "SALIDA" : "CERRADA"));
        salida->setPos(nivel2->getSalidaRect().topLeft());
        salida->setZValue(4);

        if (nivel2->getArania()) {
            QPixmap spiderPixmap = makeGamePixmap(animationTick % 30 < 15 ? ":/assets/arania_walk1.png" : ":/assets/arania_walk2.png",
                                                  QSize(92, 76),
                                                  QColor("#3d173b"),
                                                  "Arania");
            if (nivel2->getArania()->getX() > nivel2->getJugador()->getX()) {
                spiderPixmap = mirroredPixmap(spiderPixmap);
            }

            auto* arania = scene->addPixmap(spiderPixmap);
            arania->setPos(nivel2->getArania()->getX(), nivel2->getArania()->getY());
            arania->setZValue(5);
        }
    }

    Jugador* jugador = nivel->getJugador();
    const QSize playerSize = nivelUno ? QSize(68, 84) : QSize(58, 68);
    auto* playerItem = scene->addPixmap(makeGamePixmap(playerSpritePath(), playerSize, QColor("#1a7d69"), character.getName()));
    playerItem->setPos(jugador->getX(), jugador->getY());
    playerItem->setZValue(6);

    if (nivelUno) {
        auto* pole = scene->addPixmap(makeGamePixmap(":/assets/pertiga.png",
                                                    QSize(spaceCharging ? 145 : 105, 14),
                                                    QColor("#85c76b"),
                                                    ""));
        pole->setPos(jugador->getX() + 28, jugador->getY() + 28);
        pole->setRotation(spaceCharging ? -18.0 : -8.0);
        pole->setZValue(5);
    }

    auto* nameText = scene->addText(nivel->getNombre(), QFont("Arial", 20, QFont::Bold));
    nameText->setDefaultTextColor(QColor("#f7ffd7"));
    nameText->setPos(22, 18);
    nameText->setZValue(20);

    view->centerOn(jugador->getX() + jugador->getAncho() / 2.0f, jugador->getY() + jugador->getAlto() / 2.0f);
}

void GameScreen::setupAudio()
{
    ambientSound->setSource(QUrl("qrc:/assets/ambiente_jardin.wav"));
    ambientSound->setLoopCount(QSoundEffect::Infinite);
    ambientSound->setVolume(0.16f);

    collectSound->setSource(QUrl("qrc:/assets/sfx_recolectar.wav"));
    collectSound->setLoopCount(1);
    collectSound->setVolume(0.55f);

    damageSound->setSource(QUrl("qrc:/assets/sfx_danio.wav"));
    damageSound->setLoopCount(1);
    damageSound->setVolume(0.55f);
}

void GameScreen::startAmbientAudio()
{
    if (!ambientSound->isPlaying()) {
        ambientSound->play();
    }
}

void GameScreen::stopAmbientAudio()
{
    if (ambientSound->isPlaying()) {
        ambientSound->stop();
    }
}

void GameScreen::resetAudioTracking()
{
    Jugador* jugador = juego->getJugador();
    if (!jugador) {
        lastTrackedHealth = 0;
        lastTrackedNectar = 0;
        lastTrackedSemillas = 0;
        return;
    }

    lastTrackedHealth = jugador->getVidas();
    lastTrackedNectar = jugador->getNectar();
    lastTrackedSemillas = jugador->getSemillas();
}

void GameScreen::updateAudioFeedback()
{
    Jugador* jugador = juego->getJugador();
    if (!jugador) {
        return;
    }

    if (jugador->getNectar() > lastTrackedNectar || jugador->getSemillas() > lastTrackedSemillas) {
        collectSound->play();
    }

    if (jugador->getVidas() < lastTrackedHealth) {
        damageSound->play();
    }

    lastTrackedHealth = jugador->getVidas();
    lastTrackedNectar = jugador->getNectar();
    lastTrackedSemillas = jugador->getSemillas();
}

void GameScreen::updateHUD()
{
    Nivel* nivel = juego->getNivelActual();
    Jugador* jugador = juego->getJugador();
    if (!nivel || !jugador) {
        return;
    }

    levelLabel->setText(QString("Nivel %1: %2").arg(juego->getNumeroNivel()).arg(nivel->getNombre()));
    scoreLabel->setText(QString("Puntos: %1").arg(jugador->getPuntaje()));
    healthBar->setRange(0, character.getHealth());
    healthBar->setValue(jugador->getVidas());
    poleBar->setValue(static_cast<int>(jugador->getEnergiaPertiga()));

    if (auto* nivel1 = dynamic_cast<NivelUno*>(nivel)) {
        timeLabel->setText(QString("Tiempo: %1").arg(static_cast<int>(nivel1->getTiempoRestante())));
        collectiblesLabel->setText(QString("Néctar: %1").arg(jugador->getNectar()));
        poleBar->setVisible(true);
    } else {
        timeLabel->setText("Tiempo: libre");
        collectiblesLabel->setText(QString("Semillas: %1/3").arg(jugador->getSemillas()));
        poleBar->setVisible(false);
    }
}

void GameScreen::keyPressEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat()) {
        QWidget::keyPressEvent(event);
        return;
    }

    if (event->key() == Qt::Key_Escape) {
        updateTimer->stop();
        stopAmbientAudio();
        emit returnToMenu();
        return;
    }

    if (event->key() == Qt::Key_R) {
        resetGame();
        return;
    }

    pressedKeys.insert(event->key());

    if (juego->getNumeroNivel() == 1 && event->key() == Qt::Key_W) {
        juego->saltarJugador();
    }

    if (event->key() == Qt::Key_Space) {
        if (juego->getNumeroNivel() == 1) {
            spaceCharging = true;
        } else {
            impulseRequested = true;
        }
    }
}

void GameScreen::keyReleaseEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat()) {
        QWidget::keyReleaseEvent(event);
        return;
    }

    pressedKeys.remove(event->key());

    if (juego->getNumeroNivel() == 1 && event->key() == Qt::Key_Space && spaceCharging) {
        spaceCharging = false;
        juego->usarPertiga();
    }
}

void GameScreen::updateGame()
{
    constexpr float delta = 0.016f;
    ++animationTick;

    const float dx = (pressedKeys.contains(Qt::Key_D) ? 1.0f : 0.0f)
                     - (pressedKeys.contains(Qt::Key_A) ? 1.0f : 0.0f);
    const float dy = (pressedKeys.contains(Qt::Key_S) ? 1.0f : 0.0f)
                     - (pressedKeys.contains(Qt::Key_W) ? 1.0f : 0.0f);

    if (juego->getNumeroNivel() == 1) {
        juego->moverJugador(spaceCharging ? 0.0f : dx, 0.0f);
        if (spaceCharging) {
            juego->cargarPertiga(delta);
        }
    } else {
        juego->moverJugador(dx, dy);
        if (impulseRequested) {
            juego->impulsoJugador();
            impulseRequested = false;
        }
    }

    juego->actualizar(delta);
    updateAudioFeedback();
    updateHUD();

    Nivel* nivel = juego->getNivelActual();
    if (!nivel) {
        return;
    }

    if (nivel->getEstado() == EstadoNivel::Completado) {
        if (juego->getNumeroNivel() == 1) {
            juego->cambiarNivel();
            pressedKeys.clear();
            spaceCharging = false;
            resetAudioTracking();
            buildLevelScene();
            updateHUD();
            return;
        }

        updateTimer->stop();
        stopAmbientAudio();
        emit levelTwoFinished();
        return;
    }

    if (nivel->getEstado() == EstadoNivel::Perdido) {
        updateTimer->stop();
        stopAmbientAudio();
        emit gameLost();
        return;
    }

    buildLevelScene();
}

QString GameScreen::playerSpritePath() const
{
    const Jugador* jugador = juego->getJugador();
    const QString estado = jugador ? jugador->getEstado() : "idle";
    const bool walkFrame = (animationTick / 12) % 2 == 0;

    if (juego->getNumeroNivel() == 1) {
        if (estado == "charge") return character.getLateralChargeSprite();
        if (estado == "jump") return character.getLateralJumpSprite();
        if (estado == "walk") return walkFrame ? character.getLateralWalk1Sprite() : character.getLateralWalk2Sprite();
        return character.getLateralIdleSprite();
    }

    if (estado == "charge") return character.getCenitalChargeSprite();
    if (estado == "jump") return character.getCenitalJumpSprite();
    if (estado == "walk") return walkFrame ? character.getCenitalWalk1Sprite() : character.getCenitalWalk2Sprite();
    return character.getCenitalIdleSprite();
}
