#include "ui/StyledMessageBox.h"

#include <QPainter>
#include <QScreen>
#include <QGuiApplication>
#include <QResizeEvent>
#include <QPushButton>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "ui/UIConstants.h"

StyledMessageBox::StyledMessageBox(const QString& title, 
                                 const QString& message, 
                                 MessageType type,
                                 QWidget* parent)
    : QDialog(parent),
      m_backgroundPixmap(":/images/cs_background.jpg"),
      m_title(title),
      m_message(message),
      m_type(type)
{
    setWindowTitle(title);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    
    // Configuración para ventana redimensionable
    resize(UIConstants::DEFAULT_SIZE);
    setMinimumSize(UIConstants::MINIMUM_SIZE);
    centerOnScreen();
    
    setupUi();
}

void StyledMessageBox::setupUi() {
    // Estilo para los botones
    QString btnStyle = R"(
        QPushButton {
            background-color: #A9A9A9;
            color: white;
            border-radius: 5px;
            padding: 5px 15px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #8B8B8B;
        }
        QPushButton:pressed {
            background-color: #696969;
        }
    )";
    
    // Estilo para los labels
    QString titleStyle;
    QString messageStyle = "color: white; font-size: 14px;";
    
    // Configurar estilo según el tipo de mensaje
    switch (m_type) {
        case Information:
            titleStyle = "color: #4CAF50; font-size: 18px; font-weight: bold;";
            break;
        case Warning:
            titleStyle = "color: #FFC107; font-size: 18px; font-weight: bold;";
            break;
        case Error:
            titleStyle = "color: #F44336; font-size: 18px; font-weight: bold;";
            break;
    }
    
    // Crear el layout principal
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);
    
    // Panel semitransparente para el fondo
    setStyleSheet("StyledMessageBox { background-color: rgba(0, 0, 0, 150); }");
    
    // Título
    titleLabel = new QLabel(m_title, this);
    titleLabel->setStyleSheet(titleStyle);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Mensaje
    messageLabel = new QLabel(m_message, this);
    messageLabel->setStyleSheet(messageStyle);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setWordWrap(true);
    mainLayout->addWidget(messageLabel);
    
    // Espacio flexible
    mainLayout->addStretch();
    
    // Botón de OK
    okButton = new QPushButton("Aceptar", this);
    okButton->setStyleSheet(btnStyle);
    okButton->setFixedSize(120, 30);
    mainLayout->addWidget(okButton, 0, Qt::AlignCenter);
    
    // Conectar señales y slots
    connect(okButton, &QPushButton::clicked, this, &StyledMessageBox::accept);
}

void StyledMessageBox::paintEvent(QPaintEvent *event) {
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
    
    // Dibujar un panel semitransparente
    painter.fillRect(rect(), QColor(0, 0, 0, 180));
    
    // Dibujar un borde para el diálogo
    QColor borderColor;
    switch (m_type) {
        case Information:
            borderColor = QColor("#4CAF50");
            break;
        case Warning:
            borderColor = QColor("#FFC107");
            break;
        case Error:
            borderColor = QColor("#F44336");
            break;
    }
    
    painter.setPen(QPen(borderColor, 3));
    painter.drawRect(rect().adjusted(2, 2, -2, -2));
    
    QDialog::paintEvent(event);
}

void StyledMessageBox::resizeEvent(QResizeEvent *event) {
    QDialog::resizeEvent(event);
    centerOnScreen();
}

void StyledMessageBox::centerOnScreen() {
    if (QScreen *screen = QGuiApplication::primaryScreen()) {
        QRect geom = screen->availableGeometry();
        int x = (geom.width() - width()) / 2;
        int y = (geom.height() - height()) / 2;
        move(x, y);
    }
}

StyledMessageBox::~StyledMessageBox() {
    // No need to delete QObjects that have a parent
}

// Función de conveniencia para mostrar un mensaje de advertencia estilizado
void showStyledWarning(const QString& title, const QString& message, QWidget* parent) {
    StyledMessageBox msgBox(title, message, StyledMessageBox::Warning, parent);
    msgBox.exec();
}
