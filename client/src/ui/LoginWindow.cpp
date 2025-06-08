#include "ui/LoginWindow.h"

#include <QFont>
#include <QFormLayout>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMovie>
#include <QPainter>
#include <QResizeEvent>
#include <QScreen>
#include <QVBoxLayout>

#include "ui/UIConstants.h"

LoginWindow::LoginWindow(QWidget* parent):
        QDialog(parent),
        m_userEdit(new QLineEdit(this)),
        m_ipEdit(new QLineEdit(this)),
        m_portEdit(new QLineEdit(this)),
        m_connectBtn(new QPushButton("Conectar", this)),
        m_gifLabel(new QLabel(this)),
        m_backgroundPixmap(":/images/cs_background.jpg") {
    // Configuración básica de la ventana
    setWindowTitle("Login - Taller CS");
    setModal(true);
    resize(UIConstants::DEFAULT_SIZE);  // Tamaño inicial
    setMinimumSize(UIConstants::MINIMUM_SIZE);
    centerOnScreen();

    // Carga y prepara el GIF animado (logo)
    m_gifLabel->setFixedSize(100, 100);
    m_gifLabel->setStyleSheet("background: transparent;");
    QMovie* movie = new QMovie(":/animations/cs_logo.gif");
    movie->setScaledSize(m_gifLabel->size());
    m_gifLabel->setMovie(movie);
    movie->start();

    // Estilos de QLineEdit y QPushButton
    QFont editFont;
    editFont.setPointSize(11);
    m_userEdit->setFont(editFont);
    m_ipEdit->setFont(editFont);
    m_portEdit->setFont(editFont);

    m_userEdit->setPlaceholderText("Ingrese usuario");
    m_ipEdit->setPlaceholderText("Ingrese IP del servidor");
    m_portEdit->setPlaceholderText("Ingrese puerto");

    QString lineEditStyle = R"(
        QLineEdit {
            background: rgba(255, 255, 255, 0.8);
            color: black;
            border-radius: 5px;
            padding: 5px;
        }
    )";
    m_userEdit->setStyleSheet(lineEditStyle);
    m_ipEdit->setStyleSheet(lineEditStyle);
    m_portEdit->setStyleSheet(lineEditStyle);

    m_connectBtn->setCursor(Qt::PointingHandCursor);
    m_connectBtn->setFixedHeight(30);
    QFont btnFont;
    btnFont.setPointSize(11);
    btnFont.setBold(true);
    m_connectBtn->setFont(btnFont);

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
    m_connectBtn->setStyleSheet(btnStyle);

    // Creamos el formulario con QFormLayout (dentro de un contenedor)
    QWidget* formContainer = new QWidget(this);
    formContainer->setObjectName("formContainer");
    formContainer->setStyleSheet("#formContainer { background: transparent; }");
    formContainer->setFixedWidth(500);

    auto formLayout = new QFormLayout(formContainer);
    formLayout->setLabelAlignment(Qt::AlignRight);
    formLayout->setFormAlignment(Qt::AlignCenter);
    formLayout->setHorizontalSpacing(15);
    formLayout->setVerticalSpacing(12);

    QFont labelFont;
    labelFont.setPointSize(11);

    auto lblUser = new QLabel("Usuario:", formContainer);
    lblUser->setFont(labelFont);
    lblUser->setStyleSheet("color: white;");

    auto lblIP = new QLabel("IP Servidor:", formContainer);
    lblIP->setFont(labelFont);
    lblIP->setStyleSheet("color: white;");

    auto lblPort = new QLabel("Puerto:", formContainer);
    lblPort->setFont(labelFont);
    lblPort->setStyleSheet("color: white;");

    formLayout->addRow(lblUser, m_userEdit);
    formLayout->addRow(lblIP, m_ipEdit);
    formLayout->addRow(lblPort, m_portEdit);

    // Composición con QVBoxLayout para el diálogo
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Layout para centrar el GIF en la parte superior
    auto gifLayout = new QHBoxLayout;
    gifLayout->addStretch(1);
    gifLayout->addWidget(m_gifLabel);
    gifLayout->addStretch(1);
    mainLayout->addLayout(gifLayout);

    // Espacio entre GIF y formulario
    mainLayout->addSpacing(15);

    // Metemos el formContainer en un layout horizontal que lo centre
    auto centerFormLayout = new QHBoxLayout;
    centerFormLayout->addStretch(1);
    centerFormLayout->addWidget(formContainer);
    centerFormLayout->addStretch(1);
    mainLayout->addLayout(centerFormLayout);

    // Espacio fijo entre formulario y botón
    mainLayout->addSpacing(15);

    // Layout para centrar el botón justo debajo del formulario
    auto btnLayout = new QHBoxLayout;
    btnLayout->addStretch(1);
    btnLayout->addWidget(m_connectBtn);
    btnLayout->addStretch(1);
    mainLayout->addLayout(btnLayout);

    // Espacio flexible al final para empujar todo hacia arriba
    mainLayout->addStretch(1);

    setLayout(mainLayout);

    // Conexiones de señales y slots
    connect(m_connectBtn, &QPushButton::clicked, this, &LoginWindow::onConnectClicked);
}

// Cada vez que cambie el tamaño, volvemos a centrar
void LoginWindow::resizeEvent(QResizeEvent* event) { QDialog::resizeEvent(event); }

LoginWindow::~LoginWindow() {
    // Qt libera automáticamente los hijos
}

void LoginWindow::centerOnScreen() {
    // Obtener la geometría de la pantalla principal
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    // Calcular la posición para centrar la ventana
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;

    // Mover la ventana a la posición calculada
    move(x, y);
}

void LoginWindow::paintEvent(QPaintEvent* event) {
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

void LoginWindow::onConnectClicked() {
    if (m_userEdit->text().isEmpty() || m_ipEdit->text().isEmpty() ||
        m_portEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Faltan datos",
                             "Por favor ingresa usuario, puerto e IP del servidor.");
        return;
    }
    accept();
}

QString LoginWindow::userName() const { return m_userEdit->text(); }
QString LoginWindow::serverIp() const { return m_ipEdit->text(); }
QString LoginWindow::serverPort() const { return m_portEdit->text(); }
