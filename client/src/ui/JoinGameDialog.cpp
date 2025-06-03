#include "ui/JoinGameDialog.h"

#include <QPainter>
#include <QScreen>
#include <QGuiApplication>
#include <QResizeEvent>
#include <QPushButton>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>

#include "../include/client.h"
#include "ui/UIConstants.h"

// Referencia a la variable global definida en MainMenuWindow.cpp
extern Client* g_client;

JoinGameDialog::JoinGameDialog(QWidget *parent)
    : QDialog(parent),
      m_backgroundPixmap(":/images/cs_background.jpg")
{
    setWindowTitle("Unirse a Partida");
    
    // Configuración para ventana redimensionable
    resize(UIConstants::DEFAULT_SIZE);
    setMinimumSize(UIConstants::MINIMUM_SIZE);
    centerOnScreen();
    
    setupUi();
    
    // Inicializar la lista de partidas
    updateGameList();
}

void JoinGameDialog::setupUi() {
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
    
    // Crear el layout principal
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);
    
    // Título
    QLabel* titleLabel = new QLabel("Partidas Disponibles", this);
    titleLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Lista de partidas
    gameListWidget = new QListWidget(this);
    gameListWidget->setStyleSheet(listStyle);
    gameListWidget->setMinimumHeight(300);
    mainLayout->addWidget(gameListWidget);
    
    // Botón de actualizar
    refreshButton = new QPushButton("Actualizar Lista", this);
    refreshButton->setStyleSheet(btnStyle);
    refreshButton->setFixedSize(150, 30);
    mainLayout->addWidget(refreshButton, 0, Qt::AlignCenter);
    
    // Layout para los botones de acción
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(20);
    
    // Botón de unirse
    joinButton = new QPushButton("Unirse", this);
    joinButton->setStyleSheet(btnStyle);
    joinButton->setFixedSize(120, 30);
    joinButton->setEnabled(false); // Deshabilitado hasta que se seleccione una partida
    
    // Botón de cancelar
    cancelButton = new QPushButton("Cancelar", this);
    cancelButton->setStyleSheet(btnStyle);
    cancelButton->setFixedSize(120, 30);
    
    // Añadir botones al layout
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(joinButton);
    
    // Añadir layout de botones al layout principal
    mainLayout->addLayout(buttonLayout);
    
    // Conectar señales y slots
    connect(joinButton, &QPushButton::clicked, this, &JoinGameDialog::onJoinGame);
    connect(cancelButton, &QPushButton::clicked, this, &JoinGameDialog::onCancel);
    connect(refreshButton, &QPushButton::clicked, this, &JoinGameDialog::onRefreshGameList);
    connect(gameListWidget, &QListWidget::itemClicked, this, &JoinGameDialog::onGameSelected);
}

void JoinGameDialog::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (!m_backgroundPixmap.isNull()) {
        QPixmap scaled = m_backgroundPixmap.scaled(
            size(),
            Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation
        );
        int x = (width() - scaled.width()) / 2;
        int y = (height() - scaled.height()) / 2;
        painter.drawPixmap(x, y, scaled);
    }
    QDialog::paintEvent(event);
}

void JoinGameDialog::resizeEvent(QResizeEvent *event) {
    QDialog::resizeEvent(event);
    centerOnScreen();
}

void JoinGameDialog::centerOnScreen() {
    if (QScreen *screen = QGuiApplication::primaryScreen()) {
        QRect geom = screen->availableGeometry();
        int x = (geom.width() - width()) / 2;
        int y = (geom.height() - height()) / 2;
        move(x, y);
    }
}

void JoinGameDialog::onJoinGame() {
    if (!selectedGame.isEmpty() && g_client) {
        // Si hay una partida seleccionada y tenemos acceso al cliente global
        g_client->JoinMatch(selectedGame.toStdString());
        done(JoinSelected);
    } else {
        QMessageBox::warning(this, "Error", "Por favor selecciona una partida primero.");
    }
}

void JoinGameDialog::onCancel() {
    done(Cancel);
}

void JoinGameDialog::onRefreshGameList() {
    updateGameList();
}

void JoinGameDialog::onGameSelected(QListWidgetItem* item) {
    if (item) {
        selectedGame = item->text();
        joinButton->setEnabled(true);
    } else {
        selectedGame.clear();
        joinButton->setEnabled(false);
    }
}

void JoinGameDialog::updateGameList() {
    gameListWidget->clear();
    selectedGame.clear();
    joinButton->setEnabled(false);
    
    if (g_client) {
        // Obtener la lista de partidas del cliente
        gameList = g_client->refreshMatchList();
        
        // Añadir las partidas a la lista
        for (const auto& game : gameList) {
            gameListWidget->addItem(QString::fromStdString(game));
        }
        
        if (gameList.empty()) {
            QListWidgetItem* noGamesItem = new QListWidgetItem("No hay partidas disponibles");
            noGamesItem->setFlags(noGamesItem->flags() & ~Qt::ItemIsEnabled);
            gameListWidget->addItem(noGamesItem);
        }
    }
}

QString JoinGameDialog::getSelectedGame() const {
    return selectedGame;
}

JoinGameDialog::~JoinGameDialog() {
    // No need to delete QObjects that have a parent
}
