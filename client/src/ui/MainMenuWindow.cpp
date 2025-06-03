#include "ui/MainMenuWindow.h"
#include "ui_MainMenuWindow.h"

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

// Variable global para acceder al cliente desde cualquier parte
Client* g_client = nullptr;

MainMenuWindow::MainMenuWindow(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::MainMenuWindow),
      m_backgroundPixmap(":/images/cs_background.jpg")
{
    ui->setupUi(this);
    setWindowTitle("Menú Principal");

    // Configuración para ventana redimensionable
    resize(UIConstants::DEFAULT_SIZE);
    setMinimumSize(UIConstants::MINIMUM_SIZE);
    centerOnScreen();

    // Reducir el espacio vertical entre botones (spacing del QVBox)
    ui->verticalLayout->setSpacing(5);

    // Estilo “gris redondeado” y tamaño fijo de botones
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

    const int fixedWidth = 200;
    const int fixedHeight = 30;

    auto setupButton = [&](QPushButton *btn) {
        btn->setStyleSheet(btnStyle);
        btn->setFixedSize(fixedWidth, fixedHeight);
        btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        ui->verticalLayout->setAlignment(btn, Qt::AlignHCenter);
    };

    setupButton(ui->createButton);
    setupButton(ui->joinButton);
    setupButton(ui->quitButton);

    // Simplificamos la interfaz para mostrar solo los tres botones principales
    
    // Conexiones señal/slot
    connect(ui->createButton, &QPushButton::clicked, this, &MainMenuWindow::onCreateGame);
    connect(ui->joinButton,   &QPushButton::clicked, this, &MainMenuWindow::onJoinGame);
    connect(ui->quitButton,   &QPushButton::clicked, this, &MainMenuWindow::onQuit);
}

MainMenuWindow::~MainMenuWindow() {
    delete ui;
}

void MainMenuWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (!m_backgroundPixmap.isNull()) {
        QPixmap scaled = m_backgroundPixmap.scaled(
            size(),
            Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation
        );
        int x = (width()  - scaled.width()) / 2;
        int y = (height() - scaled.height()) / 2;
        painter.drawPixmap(x, y, scaled);
    }
    QDialog::paintEvent(event);
}

void MainMenuWindow::resizeEvent(QResizeEvent *event) {
    QDialog::resizeEvent(event);
    centerOnScreen();
}

void MainMenuWindow::centerOnScreen() {
    if (QScreen *screen = QGuiApplication::primaryScreen()) {
        QRect geom = screen->availableGeometry();
        int x = (geom.width()  - width()) / 2;
        int y = (geom.height() - height()) / 2;
        move(x, y);
    }
}

void MainMenuWindow::onCreateGame() {
    done(CreateGame);
}

void MainMenuWindow::onJoinGame() {
    // Simplemente indicamos que queremos unirse a una partida
    // El diálogo de selección de partidas se mostrará después
    done(JoinGame);
}

void MainMenuWindow::onQuit() {
    done(QuitApp);
}
