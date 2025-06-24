#include "../../include/ui/ScenarioSelectionDialog.h"

#include <QGuiApplication>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include <QResizeEvent>
#include <QScreen>
#include <QVBoxLayout>

#include "../../include/ui/UIConstants.h"

ScenarioSelectionDialog::ScenarioSelectionDialog(
        QWidget* parent, const std::vector<std::string>& availableScenarios):
        QDialog(parent),
        scenarios(availableScenarios),
        selectedIndex(-1),
        m_backgroundPixmap(":/images/cs_background.jpg") {
    setWindowTitle("Seleccionar Escenario");

    // Configuración para ventana redimensionable
    resize(UIConstants::DEFAULT_SIZE);
    setMinimumSize(UIConstants::MINIMUM_SIZE);
    centerOnScreen();

    setupUi();

    // Si hay escenarios disponibles, seleccionar el primero por defecto
    if (!scenarios.empty()) {
        scenarioList->setCurrentRow(0);
    }
}

void ScenarioSelectionDialog::setupUi() {
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

    // Crear layout principal con margen para mejor visualización
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);

    // Panel semi-transparente para contener todo
    QWidget* container = new QWidget(this);
    container->setObjectName("container");
    container->setStyleSheet(
            "#container { background-color: rgba(0, 0, 0, 120); border-radius: 8px; }");
    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(15, 15, 15, 15);

    // Etiqueta de instrucción
    QLabel* instructionLabel = new QLabel("Selecciona un escenario para la partida:", container);
    instructionLabel->setStyleSheet("color: white; font-size: 14px; font-weight: bold;");
    containerLayout->addWidget(instructionLabel);

    // Lista de escenarios disponibles
    scenarioList = new QListWidget(container);
    scenarioList->setStyleSheet(R"(
        QListWidget {
            background-color: rgba(30, 30, 30, 180);
            color: white;
            border: 1px solid #555;
            border-radius: 5px;
        }
        QListWidget::item {
            padding: 5px;
        }
        QListWidget::item:selected {
            background-color: #4b6eaf;
        }
    )");

    for (const auto& scenario: scenarios) {
        scenarioList->addItem(QString::fromStdString(scenario));
    }
    containerLayout->addWidget(scenarioList);

    // Botones de acción
    QHBoxLayout* buttonsLayout = new QHBoxLayout();

    selectButton = new QPushButton("Seleccionar", container);
    selectButton->setStyleSheet(btnStyle);
    selectButton->setEnabled(false);  // Deshabilitado hasta que se seleccione un escenario

    cancelButton = new QPushButton("Cancelar", container);
    cancelButton->setStyleSheet(btnStyle);

    buttonsLayout->addStretch();
    buttonsLayout->addWidget(selectButton);
    buttonsLayout->addWidget(cancelButton);
    containerLayout->addLayout(buttonsLayout);

    // Agregar el contenedor al layout principal
    mainLayout->addWidget(container);

    // Conectar señales
    connect(scenarioList, &QListWidget::currentRowChanged, this,
            &ScenarioSelectionDialog::onScenarioSelectionChanged);
    connect(selectButton, &QPushButton::clicked, this, &ScenarioSelectionDialog::onSelectClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void ScenarioSelectionDialog::centerOnScreen() {
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}

void ScenarioSelectionDialog::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), m_backgroundPixmap);
    QDialog::paintEvent(event);
}

void ScenarioSelectionDialog::resizeEvent(QResizeEvent* event) { QDialog::resizeEvent(event); }

void ScenarioSelectionDialog::onScenarioSelectionChanged(int currentRow) {
    if (currentRow >= 0 && currentRow < static_cast<int>(scenarios.size())) {
        selectedIndex = currentRow;
        selectButton->setEnabled(true);
    } else {
        selectButton->setEnabled(false);
    }
}

void ScenarioSelectionDialog::onSelectClicked() {
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(scenarios.size())) {
        accept();  // Cierra el diálogo con resultado Accept
    } else {
        QMessageBox::warning(this, "Aviso", "Debes seleccionar un escenario para continuar.");
    }
}

std::string ScenarioSelectionDialog::getSelectedScenario() const {
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(scenarios.size())) {
        return scenarios[selectedIndex];
    }
    return "";
}
