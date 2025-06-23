#include "ui/LobbyWindow.h"

#include <QGroupBox>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPainter>
#include <QPushButton>
#include <QResizeEvent>
#include <QScreen>
#include <QSizePolicy>
#include <QVBoxLayout>

#include "../include/client.h"
#include "ui/UIConstants.h"

LobbyWindow::LobbyWindow(Client* client, QWidget* parent):
        QDialog(parent),
        client(client),
        m_backgroundPixmap(":/images/cs_background.jpg"),
        isCreator(false) {
    setWindowTitle("Sala de Espera");

    // Configuración para ventana redimensionable
    resize(UIConstants::DEFAULT_SIZE);
    setMinimumSize(UIConstants::MINIMUM_SIZE);
    centerOnScreen();

    setupUi();

    // Configurar el timer para actualizar la lista de jugadores cada 2 segundos
    connect(&refreshTimer, &QTimer::timeout, this, &LobbyWindow::onRefreshPlayers);
    refreshTimer.start(2000);

    // Actualizar la lista de jugadores inmediatamente
    onRefreshPlayers();
}

void LobbyWindow::setupUi() {
    // Estilo para los botones
    QString btnStyle = R"(
        QPushButton {
            background-color: #A9A9A9;
            color: white;
            border-radius: 5px;
            padding: 5px 15px;
        }
        QPushButton:hover {
            background-color: #8B8B8B;
        }
        QPushButton:pressed {
            background-color: #696969;
        }
    )";

    // Estilo para las listas
    QString listStyle = R"(
        QListWidget {
            background-color: rgba(0, 0, 0, 150);
            color: white;
            border-radius: 5px;
            padding: 5px;
        }
        QListWidget::item {
            height: 25px;
        }
        QListWidget::item:selected {
            background-color: rgba(100, 100, 100, 150);
        }
    )";

    // Estilo para las etiquetas
    QString labelStyle = R"(
        QLabel {
            color: white;
            font-weight: bold;
            font-size: 14px;
        }
    )";

    // Crear los elementos de la UI
    titleLabel = new QLabel("Sala de Espera - Jugadores Conectados", this);
    titleLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Botones
    startButton = new QPushButton("Iniciar Partida", this);
    startButton->setStyleSheet(btnStyle);
    startButton->setFixedSize(150, 30);

    leaveButton = new QPushButton("Abandonar", this);
    leaveButton->setStyleSheet(btnStyle);
    leaveButton->setFixedSize(150, 30);

    refreshButton = new QPushButton("Actualizar", this);
    refreshButton->setStyleSheet(btnStyle);
    refreshButton->setFixedSize(150, 30);

    // Listas de jugadores
    terroristLabel = new QLabel("Terroristas", this);
    terroristLabel->setStyleSheet(labelStyle);
    terroristLabel->setAlignment(Qt::AlignCenter);

    counterTerroristLabel = new QLabel("Anti-Terroristas", this);
    counterTerroristLabel->setStyleSheet(labelStyle);
    counterTerroristLabel->setAlignment(Qt::AlignCenter);

    terroristList = new QListWidget(this);
    terroristList->setStyleSheet(listStyle);
    terroristList->setFixedHeight(250);

    counterTerroristList = new QListWidget(this);
    counterTerroristList->setStyleSheet(listStyle);
    counterTerroristList->setFixedHeight(250);

    // Crear los layouts
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Añadir el título
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    // Layout para las listas de jugadores
    QHBoxLayout* listsLayout = new QHBoxLayout();

    // Layout para terroristas
    QVBoxLayout* terroristLayout = new QVBoxLayout();
    terroristLayout->addWidget(terroristLabel, 0, Qt::AlignCenter);
    terroristLayout->addWidget(terroristList);

    // Layout para anti-terroristas
    QVBoxLayout* counterTerroristLayout = new QVBoxLayout();
    counterTerroristLayout->addWidget(counterTerroristLabel, 0, Qt::AlignCenter);
    counterTerroristLayout->addWidget(counterTerroristList);

    // Añadir las columnas al layout horizontal
    listsLayout->addLayout(terroristLayout);
    listsLayout->addLayout(counterTerroristLayout);

    // Añadir el layout de listas al layout principal
    mainLayout->addLayout(listsLayout);

    // Layout para los botones
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(leaveButton, 0, Qt::AlignCenter);
    buttonsLayout->addWidget(refreshButton, 0, Qt::AlignCenter);
    buttonsLayout->addWidget(startButton, 0, Qt::AlignCenter);

    // Añadir los botones al layout principal
    mainLayout->addLayout(buttonsLayout);

    // Conectar señales y slots
    connect(startButton, &QPushButton::clicked, this, &LobbyWindow::onStartGame);
    connect(leaveButton, &QPushButton::clicked, this, &LobbyWindow::onLeaveGame);
    connect(refreshButton, &QPushButton::clicked, this, &LobbyWindow::onRefreshPlayers);

    // Inicialmente ocultar el botón de inicio
    startButton->setVisible(false);
}

void LobbyWindow::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    if (!m_backgroundPixmap.isNull()) {
        QPixmap scaled = m_backgroundPixmap.scaled(size(), Qt::KeepAspectRatioByExpanding,
                                                   Qt::SmoothTransformation);
        int x = (width() - scaled.width()) / 2;
        int y = (height() - scaled.height()) / 2;
        painter.drawPixmap(x, y, scaled);
    }
    QDialog::paintEvent(event);
}

void LobbyWindow::resizeEvent(QResizeEvent* event) {
    QDialog::resizeEvent(event);
    centerOnScreen();
}

void LobbyWindow::centerOnScreen() {
    if (QScreen* screen = QGuiApplication::primaryScreen()) {
        QRect geom = screen->availableGeometry();
        int x = (geom.width() - width()) / 2;
        int y = (geom.height() - height()) / 2;
        move(x, y);
    }
}

void LobbyWindow::onStartGame() { done(StartGame); }

void LobbyWindow::onLeaveGame() { 
    // Cerrar la ventana y enviar el código de resultado LeaveGame
    // para que LobbyAppState lo procese correctamente
    done(LeaveGame); 
}

void LobbyWindow::onRefreshPlayers() {
    players = client->refreshPlayersList();
    updatePlayerLists();

    // Verificar si el cliente es el creador de la partida para mostrar u ocultar el botón de inicio
    // Esto es una simplificación, deberías implementar una forma de saber si el cliente es el
    // creador
    if (client->getStatus() == InGame) {
        // Si el juego ya comenzó, cerramos el diálogo con el resultado StartGame
        done(StartGame);
        return;
    }
}

void LobbyWindow::updatePlayerLists() {
    // Limpiar las listas
    terroristList->clear();
    counterTerroristList->clear();

    // Añadir jugadores a las listas correspondientes
    for (const auto& player: players) {
        QString playerName = QString::fromStdString(player.username);

        if (player.team == Team::Terrorist) {
            terroristList->addItem(playerName);
        } else {
            counterTerroristList->addItem(playerName);
        }

        // Si este cliente es el creador, mostrar el botón de inicio
        // Esto es una simplificación, necesitarías una forma de identificar al creador
        if (playerName.toStdString() == client->getUsername() && client->isCreator()) {
            startButton->setVisible(true);
            isCreator = true;
        }
    }
}

LobbyWindow::~LobbyWindow() { refreshTimer.stop(); }
