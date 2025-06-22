#include "../../include/ui/map_selection_dialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include <QResizeEvent>
#include <QGuiApplication>
#include <QScreen>

#include "../../include/ui/UIConstants.h"

MapSelectionDialog::MapSelectionDialog(QWidget* parent, const std::vector<MapInfo>& availableMaps) 
    : QDialog(parent), maps(availableMaps), selectedIndex(-1), m_backgroundPixmap(":/images/cs_background.jpg") {
    setWindowTitle("Seleccionar Mapa");
    
    // Configuración para ventana redimensionable
    resize(UIConstants::DEFAULT_SIZE);
    setMinimumSize(UIConstants::MINIMUM_SIZE);
    centerOnScreen();
    
    setupUi();
    
    // Si hay mapas disponibles, seleccionar el primero por defecto
    if (!maps.empty()) {
        mapList->setCurrentRow(0);
    }
}

void MapSelectionDialog::setupUi() {
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
    container->setStyleSheet("#container { background-color: rgba(0, 0, 0, 120); border-radius: 8px; }");
    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(15, 15, 15, 15);
    
    // Etiqueta de instrucción
    QLabel* instructionLabel = new QLabel("Selecciona un mapa para la partida:", container);
    instructionLabel->setStyleSheet("color: white; font-size: 14px; font-weight: bold;");
    containerLayout->addWidget(instructionLabel);
    
    // Lista de mapas disponibles
    mapList = new QListWidget(container);
    mapList->setStyleSheet(R"(
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
    
    for (const auto& map : maps) {
        mapList->addItem(QString::fromStdString(map.getName()));
    }
    containerLayout->addWidget(mapList);
    
    // Etiqueta para mostrar la descripción del mapa
    descriptionLabel = new QLabel("", container);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setMinimumHeight(60);
    descriptionLabel->setStyleSheet("color: white; background-color: rgba(30, 30, 30, 150); padding: 8px; border-radius: 5px;");
    containerLayout->addWidget(descriptionLabel);
    
    // Botones de acción
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    
    selectButton = new QPushButton("Seleccionar", container);
    selectButton->setStyleSheet(btnStyle);
    selectButton->setEnabled(false);  // Deshabilitado hasta que se seleccione un mapa
    
    cancelButton = new QPushButton("Cancelar", container);
    cancelButton->setStyleSheet(btnStyle);
    
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(selectButton);
    buttonsLayout->addWidget(cancelButton);
    containerLayout->addLayout(buttonsLayout);
    
    // Agregar el contenedor al layout principal
    mainLayout->addWidget(container);
    
    // Conectar señales
    connect(mapList, &QListWidget::currentRowChanged, this, &MapSelectionDialog::onMapSelectionChanged);
    connect(selectButton, &QPushButton::clicked, this, &MapSelectionDialog::onSelectClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void MapSelectionDialog::centerOnScreen() {
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}

void MapSelectionDialog::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), m_backgroundPixmap);
    QDialog::paintEvent(event);
}

void MapSelectionDialog::resizeEvent(QResizeEvent* event) {
    QDialog::resizeEvent(event);
}

void MapSelectionDialog::onMapSelectionChanged(int currentRow) {
    if (currentRow >= 0 && currentRow < static_cast<int>(maps.size())) {
        descriptionLabel->setText(QString::fromStdString(maps[currentRow].getDescription()));
        selectedIndex = currentRow;
        selectButton->setEnabled(true);
    } else {
        descriptionLabel->clear();
        selectButton->setEnabled(false);
    }
}

void MapSelectionDialog::onSelectClicked() {
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(maps.size())) {
        accept();  // Cierra el diálogo con resultado Accept
    } else {
        QMessageBox::warning(this, "Aviso", "Debes seleccionar un mapa para continuar.");
    }
}

int MapSelectionDialog::getSelectedMapIndex() const {
    return selectedIndex;
}

std::string MapSelectionDialog::getSelectedMapName() const {
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(maps.size())) {
        return maps[selectedIndex].getName();
    }
    return "";
}
