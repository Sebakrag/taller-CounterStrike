// NameInputDialog.cpp

#include "ui/NameInputDialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QScreen>
#include <QGuiApplication>
#include "ui/UIConstants.h"

NameInputDialog::NameInputDialog(const QString &title,
                                 const QString &labelText,
                                 QWidget *parent)
    : QDialog(parent),
      m_backgroundPixmap(":/images/cs_background.jpg")
{
    // Hacemos la ventana semitransparente (para ver el fondo dibujado).
    setAttribute(Qt::WA_TranslucentBackground);
    // Quitamos el botón de “?” de ayuda (solo barra de título y cerrar)
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(title);

    // Construimos UI (solo pasamos labelText, el "title" ya se aplicó arriba)
    setupUi(labelText);

    // Tamaño mínimo para que no quede demasiado pequeño
    resize(UIConstants::DEFAULT_SIZE);
    setMinimumSize(UIConstants::MINIMUM_SIZE);

    // Al mostrarla por primera vez, la centramos en la pantalla principal
    centerOnScreen();
}

NameInputDialog::~NameInputDialog() {
    // Qt borra automáticamente todos los hijos
}

void NameInputDialog::setupUi(const QString &labelText) {
    
    // Creamos los widgets
    m_label = new QLabel(labelText, this);
    
    // Centramos el texto y color blanco
    m_label->setAlignment(Qt::AlignHCenter);
    m_label->setStyleSheet("color: white; font-size: 11pt;");
    m_label->setFixedSize(400, 30);
    m_label->setWordWrap(true);

    m_lineEdit = new QLineEdit(this);
    // Fijamos un tamaño exacto: por ejemplo 250 px de ancho, 30 px de alto
    m_lineEdit->setFixedSize(250, 30);
    // Estilo translúcido/redondeado, texto negro
    m_lineEdit->setStyleSheet(R"(
        QLineEdit {
            background: rgba(255, 255, 255, 0.8);
            border-radius: 5px;
            padding: 5px;
            color: black;
            font-size: 11pt;
        }
    )");
    m_lineEdit->setPlaceholderText("Ingrese texto aquí");

    m_okButton = new QPushButton("OK", this);
    m_cancelButton = new QPushButton("Cancelar", this);
    // Mismo estilo gris/redondeado que en el resto de la app
    const QString btnStyle = R"(
        QPushButton {
            background-color: #A9A9A9;
            color: white;
            border-radius: 5px;
            padding: 5px 15px;
            font-size: 11pt;
        }
        QPushButton:hover {
            background-color: #8B8B8B;
        }
        QPushButton:pressed {
            background-color: #696969;
        }
    )";
    m_okButton->setStyleSheet(btnStyle);
    m_cancelButton->setStyleSheet(btnStyle);
    m_okButton->setFixedHeight(30);
    m_cancelButton->setFixedHeight(30);

    // Layouts
    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addStretch(1);

    auto *mainLayout = new QVBoxLayout(this);
    // Márgenes interiores de 20px por cada lado
    mainLayout->setContentsMargins(20, 20, 20, 20);
    // Espacio vertical entre elementos
    mainLayout->setSpacing(15);

    // – Etiqueta centrada –
    mainLayout->addWidget(m_label, 0, Qt::AlignHCenter);
    // – Cuadro de texto centrado –
    mainLayout->addWidget(m_lineEdit, 0, Qt::AlignHCenter);
    // – Botones centrados –
    mainLayout->addLayout(buttonLayout);
    // Espacio flexible al final para empujar todo hacia arriba
    mainLayout->addStretch(1);

    setLayout(mainLayout);

    // Conectamos los slots de los botones
    connect(m_okButton, &QPushButton::clicked, this, &NameInputDialog::onOkClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &NameInputDialog::onCancelClicked);
}

void NameInputDialog::onOkClicked() {
    QString text = m_lineEdit->text().trimmed();
    if (text.isEmpty()) {
        // Si el campo está vacío, no hacemos nada (no se cierra)
        return;
    }
    m_accepted = true;
    accept();
}

void NameInputDialog::onCancelClicked() {
    m_accepted = false;
    reject();
}

QString NameInputDialog::textValue() const {
    return m_lineEdit->text();
}

void NameInputDialog::paintEvent(QPaintEvent *event) {
    // Pintamos la misma imagen de fondo que en Login/MainMenu
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (!m_backgroundPixmap.isNull()) {
        // Escalamos la imagen al tamaño completo del diálogo
        QPixmap scaled = m_backgroundPixmap.scaled(
            size(),
            Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation
        );
        int x = (width()  - scaled.width()) / 2;
        int y = (height() - scaled.height()) / 2;
        painter.drawPixmap(x, y, scaled);
    }
    // Luego dejamos que QDialog pinte sus hijos (etiqueta, QLineEdit, botones)
    QDialog::paintEvent(event);
}

void NameInputDialog::centerOnScreen() {
    if (QScreen *screen = QGuiApplication::primaryScreen()) {
        QRect geom = screen->availableGeometry();
        int x = (geom.width()  - width()) / 2;
        int y = (geom.height() - height()) / 2;
        move(x, y);
    }
}
