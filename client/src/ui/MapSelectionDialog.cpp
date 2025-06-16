#include "client/include/ui/MapSelectionDialog.h"
#include <QPainter>
#include <QPixmap>
#include <QColor>

MapSelectionDialog::MapSelectionDialog(const std::vector<ClientMapInfo>& maps, QWidget* parent)
    : QDialog(parent), availableMaps(maps), selectedIndex(-1) {
    setWindowTitle("Selección de Mapa");
    setMinimumSize(500, 400);
    
    setupUI();
    
    // Si hay mapas disponibles, seleccionar el primero por defecto
    if (!availableMaps.empty() && mapList->count() > 0) {
        mapList->setCurrentRow(0);
        selectedIndex = 0;
        selectedFile = QString::fromStdString(availableMaps[0].fileName);
        updatePreview();
    }
}

void MapSelectionDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QLabel* titleLabel = new QLabel("Selecciona un mapa para la partida:", this);
    titleLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);
    
    // Vista de lista para los mapas
    mapList = new QListWidget(this);
    mapList->setStyleSheet("QListWidget { background-color: #333; color: white; border: 1px solid #555; }"
                         "QListWidget::item { padding: 5px; }"
                         "QListWidget::item:selected { background-color: #555; }");
    
    // Agregar los mapas a la lista
    for (const ClientMapInfo& map : availableMaps) {
        QString displayName = QString("%1 (%2)").arg(
            QString::fromStdString(map.displayName),
            QString::fromStdString(map.fileName));
        QListWidgetItem* item = new QListWidgetItem(displayName);
        mapList->addItem(item);
    }
    
    connect(mapList, &QListWidget::itemClicked, this, &MapSelectionDialog::onMapSelected);
    mainLayout->addWidget(mapList);
    
    // Vista previa del mapa (simplemente un placeholder visual)
    previewLabel = new QLabel(this);
    previewLabel->setMinimumSize(450, 200);
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setStyleSheet("background-color: #222; border: 1px solid #555;");
    previewLabel->setText("Vista previa no disponible");
    mainLayout->addWidget(previewLabel);
    
    // Botones para seleccionar o cancelar
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    selectButton = new QPushButton("Seleccionar", this);
    selectButton->setStyleSheet("background-color: #2d6d2d; color: white; padding: 5px 15px;");
    connect(selectButton, &QPushButton::clicked, this, &MapSelectionDialog::accept);
    
    cancelButton = new QPushButton("Cancelar", this);
    cancelButton->setStyleSheet("background-color: #6d2d2d; color: white; padding: 5px 15px;");
    connect(cancelButton, &QPushButton::clicked, this, &MapSelectionDialog::reject);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(selectButton);
    buttonLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
}

void MapSelectionDialog::onMapSelected(QListWidgetItem* item) {
    int row = mapList->row(item);
    if (row >= 0 && row < static_cast<int>(availableMaps.size())) {
        selectedIndex = row;
        selectedFile = QString::fromStdString(availableMaps[row].fileName);
        updatePreview();
    }
}

void MapSelectionDialog::updatePreview() {
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(availableMaps.size())) {
        // Crear una vista previa básica del mapa (placeholder)
        QPixmap preview(previewLabel->size());
        preview.fill(QColor(50, 50, 50));
        
        QPainter painter(&preview);
        painter.setPen(QPen(Qt::white));
        painter.setFont(QFont("Arial", 12));
        
        const ClientMapInfo& map = availableMaps[selectedIndex];
        QString mapName = QString::fromStdString(map.displayName);
        QString mapDesc = QString("Nombre de archivo: %1").arg(QString::fromStdString(map.fileName));
        
        // Dibujar texto en la vista previa
        painter.drawText(preview.rect().adjusted(10, 10, -10, -30), 
                        Qt::AlignCenter | Qt::TextWordWrap, 
                        mapName);
        
        painter.drawText(preview.rect().adjusted(10, 30, -10, -10), 
                        Qt::AlignCenter | Qt::TextWordWrap, 
                        mapDesc);
                        
        previewLabel->setPixmap(preview);
    }
}

void MapSelectionDialog::accept() {
    if (selectedIndex >= 0) {
        QDialog::accept();
    }
}

QString MapSelectionDialog::selectedMapFileName() const {
    return selectedFile;
}

int MapSelectionDialog::selectedMapIndex() const {
    return selectedIndex;
}
