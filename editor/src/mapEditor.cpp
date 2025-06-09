#include "mapEditor.h"
#include "dragAndDrop.h"
#include "backgroundItem.h"
#include <iostream>
#include <QScrollBar>
#include <QCoreApplication>
#include <QPainter>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QDir>

MapEditor::MapEditor(QWidget *parent) : QMainWindow(parent), currentBackground(nullptr), currentTerrainType(0)
{
    // Configurar una escena de 1000x1000 para el mapa de tiles
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1000, 1000); // Tamaño exacto para mapa de 1000x1000
    
    // Inicializar el sistema de tiles
    currentTileId = -1;
    tileButtons = new QButtonGroup(this);

    view = new QGraphicsView(scene, this);

    view->setRenderHint(QPainter::Antialiasing, true);
    view->setRenderHint(QPainter::SmoothPixmapTransform, true);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    // Configurar scrollbars
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded); // Habilitamos scrolling
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);   // Habilitamos scrolling

    view->setSceneRect(scene->sceneRect());
    view->setFixedSize(800, 600); // Tamaño de la ventana de visualización

    // Configurar tamaño de elementos
    widthBeam = scene->sceneRect().width() / 10;

    // Panel de herramientas lateral
    QWidget *toolPanel = new QWidget(this);
    QVBoxLayout *toolLayout = new QVBoxLayout(toolPanel);
    
    // Sección de terreno
    QGroupBox *terrainGroup = new QGroupBox("Tipo de Terreno", toolPanel);
    QVBoxLayout *terrainLayout = new QVBoxLayout(terrainGroup);
    
    QComboBox *terrainSelector = new QComboBox(terrainGroup);
    terrainSelector->addItem("Desierto");            // TerrainType::DESERT
    terrainSelector->addItem("Pueblito Azteca");     // TerrainType::AZTEC_VILLAGE
    terrainSelector->addItem("Zona Entrenamiento");  // TerrainType::TRAINING_ZONE
    
    terrainLayout->addWidget(terrainSelector);
    terrainGroup->setLayout(terrainLayout);
    
    // Sección de equipos
    QGroupBox *teamsGroup = new QGroupBox("Equipos", toolPanel);
    QVBoxLayout *teamsLayout = new QVBoxLayout(teamsGroup);
    
    QPushButton *addCTButton = new QPushButton("Añadir Zona CT", teamsGroup);
    QPushButton *addTButton = new QPushButton("Añadir Zona T", teamsGroup);
    
    teamsLayout->addWidget(addCTButton);
    teamsLayout->addWidget(addTButton);
    teamsGroup->setLayout(teamsLayout);
    
    // Sección de objetos
    QGroupBox *objectsGroup = new QGroupBox("Objetos", toolPanel);
    QVBoxLayout *objectsLayout = new QVBoxLayout(objectsGroup);
    
    QPushButton *addBoxButton = new QPushButton("Añadir Estructura", objectsGroup);
    QPushButton *addBombZoneButton = new QPushButton("Añadir Zona Bomba", objectsGroup);
    
    objectsLayout->addWidget(addBoxButton);
    objectsLayout->addWidget(addBombZoneButton);
    objectsGroup->setLayout(objectsLayout);
    
    // Sección de armas
    QGroupBox *weaponsGroup = new QGroupBox("Armas", toolPanel);
    QVBoxLayout *weaponsLayout = new QVBoxLayout(weaponsGroup);
    
    QPushButton *addPistolButton = new QPushButton("Añadir Pistola", weaponsGroup);
    QPushButton *addRifleButton = new QPushButton("Añadir Rifle", weaponsGroup);
    QPushButton *addSniperButton = new QPushButton("Añadir Sniper", weaponsGroup);
    
    weaponsLayout->addWidget(addPistolButton);
    weaponsLayout->addWidget(addRifleButton);
    weaponsLayout->addWidget(addSniperButton);
    weaponsGroup->setLayout(weaponsLayout);
    
    // Sección de archivo
    QGroupBox *fileGroup = new QGroupBox("Archivo", toolPanel);
    QVBoxLayout *fileLayout = new QVBoxLayout(fileGroup);
    
    QPushButton *loadMapButton = new QPushButton("Cargar Mapa", fileGroup);
    QPushButton *saveMapButton = new QPushButton("Guardar Mapa", fileGroup);
    
    fileLayout->addWidget(loadMapButton);
    fileLayout->addWidget(saveMapButton);
    fileGroup->setLayout(fileLayout);
    
    // Sección de control de zoom
    QGroupBox *zoomGroup = new QGroupBox("Control de Zoom", toolPanel);
    QHBoxLayout *zoomLayout = new QHBoxLayout(zoomGroup);
    
    QPushButton *zoomInButton = new QPushButton("+", zoomGroup);
    QPushButton *zoomOutButton = new QPushButton("-", zoomGroup);
    zoomInButton->setFixedWidth(40);
    zoomOutButton->setFixedWidth(40);
    
    zoomLayout->addWidget(zoomInButton);
    zoomLayout->addWidget(zoomOutButton);
    zoomGroup->setLayout(zoomLayout);
    
    // Sección de selección de tiles (se llenará dinámicamente)
    tilesGroup = new QGroupBox("Tiles disponibles", toolPanel);
    QVBoxLayout *tilesLayout = new QVBoxLayout(tilesGroup);
    
    // Añadir un área de scroll para los tiles
    tilesScrollArea = new QScrollArea(tilesGroup);
    tilesScrollArea->setWidgetResizable(true);
    tilesScrollArea->setMinimumHeight(200);
    tilesScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    // Widget para contener los botones de tiles
    QWidget *tilesContainer = new QWidget();
    new QGridLayout(tilesContainer); // Aplicar layout directamente, no necesitamos guardar la referencia
    tilesScrollArea->setWidget(tilesContainer);
    
    tilesLayout->addWidget(tilesScrollArea);
    tilesGroup->setLayout(tilesLayout);
    
    // Añadir todas las secciones al panel de herramientas
    toolLayout->addWidget(terrainGroup);
    toolLayout->addWidget(tilesGroup); // Añadir el grupo de tiles
    toolLayout->addWidget(teamsGroup);
    toolLayout->addWidget(objectsGroup);
    toolLayout->addWidget(weaponsGroup);
    toolLayout->addWidget(zoomGroup);
    toolLayout->addWidget(fileGroup);
    toolLayout->addStretch(1); // Espacio flexible al final
    
    // Conectar señales
    connect(terrainSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MapEditor::backgroundSelection);
    
    connect(addCTButton, &QPushButton::clicked, this, &MapEditor::addTeamSpawnCT);
    connect(addTButton, &QPushButton::clicked, this, &MapEditor::addTeamSpawnT);
    
    connect(addBoxButton, &QPushButton::clicked, this, &MapEditor::addBox);
    connect(addBombZoneButton, &QPushButton::clicked, this, &MapEditor::addBombZone);
    
    connect(addPistolButton, &QPushButton::clicked, this, [this](){ this->addWeapon(PISTOL); });
    connect(addRifleButton, &QPushButton::clicked, this, [this](){ this->addWeapon(RIFLE); });
    connect(addSniperButton, &QPushButton::clicked, this, [this](){ this->addWeapon(SNIPER); });
    
    connect(loadMapButton, &QPushButton::clicked, this, &MapEditor::loadMapClicked);
    connect(saveMapButton, &QPushButton::clicked, this, &MapEditor::generarMapaClicked);
    
    // Conectar botones de zoom
    connect(zoomInButton, &QPushButton::clicked, this, &MapEditor::zoomIn);
    connect(zoomOutButton, &QPushButton::clicked, this, &MapEditor::zoomOut);
    
    // Instalamos un event filter en la vista para capturar clicks
    view->viewport()->installEventFilter(this);
    
    // Cargar los tiles disponibles
    loadAvailableTiles();
    
    // Inicializar con el fondo vacío con cuadrícula
    backgroundSelection(0);
    
    // Crear el layout principal con el panel de herramientas y la vista
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(toolPanel);
    mainLayout->addWidget(view, 1); // La vista toma el espacio disponible
    
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    
    // Configurar tamaño de la ventana principal
    resize(1024, 700);
    setWindowTitle("Counter-Strike Editor de Mapas");
    
    // Inicializar con el fondo de desierto por defecto
    backgroundSelection(0);
}

void MapEditor::addBox()
{
    // Añadir una estructura sólida
    QPixmap boxPixmap(getResourcePath(SOLID_STRUCTURE));
    DragAndDrop *boxItem = new DragAndDrop(boxPixmap, 1.0, scene);
    boxItem->setTipoElemento(SOLID_STRUCTURE);
    boxItem->setStructureType(0); // Tipo por defecto
    boxItem->setPixmap(boxPixmap.scaledToWidth(widthBeam));
    scene->addItem(boxItem);
}

void MapEditor::addSpawn()
{
    // Este método se mantiene por compatibilidad
    // pero ahora usamos los métodos específicos para cada equipo
    addTeamSpawnCT();
}

void MapEditor::addTeamSpawnCT()
{
    // Añadir una zona de inicio para Counter-Terrorists
    QPixmap ctPixmap(getResourcePath(TEAM_SPAWN_CT));
    DragAndDrop *ctItem = new DragAndDrop(ctPixmap, 0.8, scene);
    ctItem->setTipoElemento(TEAM_SPAWN_CT);
    ctItem->setTeamId(0); // CT = 0
    scene->addItem(ctItem);
}

void MapEditor::addTeamSpawnT()
{
    // Añadir una zona de inicio para Terrorists
    QPixmap tPixmap(getResourcePath(TEAM_SPAWN_T));
    DragAndDrop *tItem = new DragAndDrop(tPixmap, 0.8, scene);
    tItem->setTipoElemento(TEAM_SPAWN_T);
    tItem->setTeamId(1); // T = 1
    scene->addItem(tItem);
}

void MapEditor::addBombZone()
{
    // Añadir una zona para plantar bombas
    QPixmap bombZonePixmap(getResourcePath(BOMB_ZONE));
    DragAndDrop *bombZoneItem = new DragAndDrop(bombZonePixmap, 1.0, scene);
    bombZoneItem->setTipoElemento(BOMB_ZONE);
    bombZoneItem->setBombZoneSize(QSizeF(50, 50)); // Tamaño por defecto
    scene->addItem(bombZoneItem);
}

void MapEditor::addWeapon(int weaponType)
{
    // Añadir un arma al mapa
    QPixmap weaponPixmap(getResourcePath(WEAPON, weaponType));
    DragAndDrop *weaponItem = new DragAndDrop(weaponPixmap, 0.6, scene);
    weaponItem->setTipoElemento(WEAPON);
    weaponItem->setWeaponType(weaponType);
    scene->addItem(weaponItem);
}

void MapEditor::backgroundSelection(int index) 
{
    currentTerrainType = index; // Guardar el tipo de terreno seleccionado
    QString basePath = getResourcesPath();
    QString imagePath;
    
    switch (index) {
        case DESERT: // Desierto
            imagePath = basePath + "backgrounds/desert.png";
            break;
        case AZTEC_VILLAGE: // Pueblito Azteca
            imagePath = basePath + "backgrounds/aztec.png";
            break;
        case TRAINING_ZONE: // Zona de entrenamiento
            imagePath = basePath + "backgrounds/training.png";
            break;
        default:
            imagePath = basePath + "backgrounds/desert.png"; // Por defecto desierto
            break;
    }
    
    qDebug() << "Intentando cargar imagen desde: " << imagePath;
    
    // Usar QImage primero, que es más flexible con formatos
    QImage image(imagePath);
    QPixmap backgroundPixmap;
    backgroundPixmap = QPixmap::fromImage(image);

    // Si la imagen está vacía, intentar con un fondo genérico
    if (image.isNull() || backgroundPixmap.isNull()) {
        qWarning() << "No se pudo cargar la imagen de fondo desde: " << imagePath << ", usando fondo genérico.";
        
        // Crear un pixmap genérico con un patrón de cuadrícula
        backgroundPixmap = QPixmap(1024, 1024);
        backgroundPixmap.fill(getTerrainColor(index));
        
        // Dibujar una cuadrícula para mejorar la visualización
        QPainter painter(&backgroundPixmap);
        painter.setPen(QPen(Qt::darkGray, 1));
        int gridSize = 32;
        for (int i = 0; i <= backgroundPixmap.width(); i += gridSize) {
            painter.drawLine(i, 0, i, backgroundPixmap.height());
        }
        for (int i = 0; i <= backgroundPixmap.height(); i += gridSize) {
            painter.drawLine(0, i, backgroundPixmap.width(), i);
        }
    } else {
        qDebug() << "Imagen de fondo cargada exitosamente con dimensiones: " 
                << image.width() << "x" << image.height();
    }

    // Asegurarse de que la escena sea lo suficientemente grande para la imagen
    QRectF sceneRect = scene->sceneRect();
    qDebug() << "Dimensiones de la imagen: " << backgroundPixmap.width() << "x" << backgroundPixmap.height();
    qDebug() << "Dimensiones actuales de la escena: " << sceneRect.width() << "x" << sceneRect.height();
    
    // Ajustar el tamaño de la escena si la imagen es más grande
    if (!backgroundPixmap.isNull() && 
        (backgroundPixmap.width() > sceneRect.width() || 
         backgroundPixmap.height() > sceneRect.height())) {
        scene->setSceneRect(0, 0, 
                         qMax(static_cast<int>(sceneRect.width()), backgroundPixmap.width()),
                         qMax(static_cast<int>(sceneRect.height()), backgroundPixmap.height()));
        qDebug() << "Escena redimensionada a: " << scene->sceneRect().width() << "x" << scene->sceneRect().height();
    }
    
    // Asegurar que las barras de desplazamiento estén habilitadas
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    // Aplicar un zoom mayor para acercar la vista
    view->resetTransform();
    float zoomFactor = 2.0; // Valor mayor para un zoom más cercano
    view->scale(zoomFactor, zoomFactor);
    
    // Asegurar que el scroll comience en la posición superior izquierda (0,0)
    view->horizontalScrollBar()->setValue(0);
    view->verticalScrollBar()->setValue(0);
    qDebug() << "Scroll posicionado en inicio (0,0)";

    if (currentBackground) {
        currentBackground->setPixmap(backgroundPixmap);
    } else {
        currentBackground = new BackgroundItem(backgroundPixmap, 1.0);
        scene->addItem(currentBackground);
        currentBackground->setZValue(-1); // Asegurar que esté detrás de todos los elementos
    }
    
    // Actualizar la interfaz de usuario según el tipo de terreno
    updateTerrainUI(index);
}

// Método para aumentar el zoom
void MapEditor::zoomIn() {
    // Incrementar el zoom en 25%
    currentZoomFactor += 0.25;
    // Limitar el zoom máximo
    if (currentZoomFactor > 4.0) {
        currentZoomFactor = 4.0;
    }
    
    qDebug() << "Zoom aumentado a: " << currentZoomFactor;
    applyZoom();
}

// Método para disminuir el zoom
void MapEditor::zoomOut() {
    // Reducir el zoom en 25%
    currentZoomFactor -= 0.25;
    // Limitar el zoom mínimo
    if (currentZoomFactor < 0.5) {
        currentZoomFactor = 0.5;
    }
    
    qDebug() << "Zoom reducido a: " << currentZoomFactor;
    applyZoom();
}

// Método para aplicar el factor de zoom actual
void MapEditor::applyZoom() {
    // Guardar la posición actual del centro de la vista
    QPointF centerPoint = view->mapToScene(view->viewport()->width() / 2, view->viewport()->height() / 2);
    
    // Aplicar la transformación
    view->resetTransform();
    view->scale(currentZoomFactor, currentZoomFactor);
    
    // Centrar en el mismo punto después del zoom
    view->centerOn(centerPoint);
    
    qDebug() << "Aplicado factor de zoom: " << currentZoomFactor;
}

// Obtener un color representativo para cada tipo de terreno
QColor MapEditor::getTerrainColor(int terrainType)
{
    switch (terrainType) {
        case DESERT:
            return QColor(194, 178, 128); // Color arena
        case AZTEC_VILLAGE:
            return QColor(120, 145, 93);  // Verde azteca
        case TRAINING_ZONE:
            return QColor(150, 150, 150); // Gris entrenamiento
        default:
            return QColor(194, 178, 128); // Arena por defecto
    }
}

// Implementación de funciones de escala
float MapEditor::pixelToWorldX(float pixelX, float elementWidth) {
    // elementWidth no se usa actualmente pero podría usarse para ajustes de posición
    (void)elementWidth; // Evitar advertencia de parámetro no utilizado
    return pixelX / 25.6f;
}

float MapEditor::pixelToWorldY(float pixelY, float elementHeight) {
    // elementHeight no se usa actualmente pero podría usarse para ajustes de posición
    (void)elementHeight; // Evitar advertencia de parámetro no utilizado
    return pixelY / 25.6f;
}

float MapEditor::worldToPixelX(float worldX) {
    return worldX * 25.6f;
}

float MapEditor::worldToPixelY(float worldY) {
    return worldY * 25.6f;
}

// Método para obtener la ruta base a los recursos
QString MapEditor::getResourcesPath() {
    // Directorio actual desde donde se ejecuta la aplicación
    QDir currentDir = QDir::current();
    qDebug() << "Directorio actual:" << currentDir.absolutePath();
    
    // Ruta al directorio editor/resources
    QString editorResourcesPath = currentDir.absolutePath() + "/../editor/resources/";
    
    // Verificar si la carpeta existe
    if (QDir(editorResourcesPath).exists()) {
        qDebug() << "Usando ruta de recursos:" << editorResourcesPath;
        return editorResourcesPath;
    }
    
    // Verificar si existe en la ruta relativa al directorio actual
    if (QDir("../editor/resources/").exists()) {
        qDebug() << "Usando ruta de recursos relativa: ../editor/resources/";
        return "../editor/resources/";
    }
    
    // Alternativa: intentar con una ruta directa a resources
    if (QDir("resources/").exists()) {
        qDebug() << "Usando ruta de recursos simple: resources/";
        return "resources/";
    }
    
    // Último intento: buscar en el directorio de la aplicación
    QString appDirPath = QCoreApplication::applicationDirPath() + "/resources/";
    if (QDir(appDirPath).exists()) {
        qDebug() << "Usando ruta de recursos desde directorio de aplicación:" << appDirPath;
        return appDirPath;
    }
    
    qWarning() << "No se encontró la carpeta de recursos. Usando ruta predeterminada.";
    return "resources/";
}

// Método para obtener la ruta completa de un recurso según su tipo
QString MapEditor::getResourcePath(int elementType, int subType) {
    QString basePath = getResourcesPath();
    
    switch (elementType) {
        case TEAM_SPAWN_CT:
            return basePath + "player/ct.png";
            
        case TEAM_SPAWN_T:
            return basePath + "player/t.png";
            
        case BOMB_ZONE:
            return basePath + "objects/bomb_zone.png";
            
        case SOLID_STRUCTURE:
            // Podría tener diferentes tipos de estructuras según subType
            return basePath + "objects/box.png";
            
        case WEAPON:
            // Diferentes tipos de armas según el subType
            switch (subType) {
                case 0: // Pistola
                    return basePath + "weapons/pistol.png";
                case 1: // Rifle
                    return basePath + "weapons/rifle.png";
                case 2: // Sniper
                    return basePath + "weapons/sniper.png";
                case 3: // Shotgun
                    return basePath + "weapons/shotgun.png";
                default:
                    return basePath + "weapons/pistol.png";
            }
            
        default:
            qWarning() << "Tipo de elemento desconocido:" << elementType;
            return "";
    }
}

// Método para cargar los tiles disponibles en gfx/tiles
void MapEditor::loadAvailableTiles() {
    // Ruta a los tiles del juego (ahora usando el sistema de recursos)
    QString tilesPath = getResourcesPath() + "tiles/";
    QDir tilesDir(tilesPath);
    
    qDebug() << "Buscando tiles en:" << tilesDir.absolutePath();
    
    // Verificar si el directorio existe
    if (!tilesDir.exists()) {
        qWarning() << "No se encontró el directorio de tiles:" << tilesPath;
        
        // Intento alternativo con la ruta antigua si la nueva no existe
        tilesPath = "../gfx/tiles/";
        tilesDir.setPath(tilesPath);
        
        if (!tilesDir.exists()) {
            qWarning() << "Tampoco se encontró la ruta alternativa de tiles:" << tilesPath;
            return;
        } else {
            qDebug() << "Usando ruta alternativa para tiles:" << tilesPath;
        }
    }
    
    // Filtrar por archivos BMP y PNG
    QStringList filters;
    filters << "*.bmp" << "*.png";
    tilesDir.setNameFilters(filters);
    
    // Obtener la lista de archivos
    QStringList tileFiles = tilesDir.entryList(filters, QDir::Files);
    qDebug() << "Tiles encontrados:" << tileFiles.count();
    
    // Limpiar los widgets existentes en el área de scroll
    QWidget* tilesContainer = tilesScrollArea->widget();
    QGridLayout* tilesGridLayout = qobject_cast<QGridLayout*>(tilesContainer->layout());
    
    // Eliminar botones existentes
    QList<QAbstractButton*> buttons = tileButtons->buttons();
    for (QAbstractButton* button : buttons) {
        tileButtons->removeButton(button);
        delete button;
    }
    tilePixmaps.clear();
    
    // Cargar cada archivo de tile
    int tileId = 0;
    int row = 0;
    int col = 0;
    
    for (const QString& tileFile : tileFiles) {
        // Solo procesar archivos de imagen
        if (tileFile.endsWith(".bmp", Qt::CaseInsensitive) || 
            tileFile.endsWith(".png", Qt::CaseInsensitive)) {
            
            QString tilePath = tilesPath + tileFile;
            QPixmap tilePixmap(tilePath);
            
            if (!tilePixmap.isNull()) {
                // Almacenar el pixmap
                tilePixmaps[tileId] = tilePixmap;
                
                // Crear un botón con una miniatura del tile
                QPushButton* tileButton = new QPushButton();
                tileButton->setFixedSize(40, 40);
                
                // Crear una versión escalada para el botón
                QPixmap scaledPixmap = tilePixmap.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                tileButton->setIcon(QIcon(scaledPixmap));
                tileButton->setIconSize(QSize(36, 36));
                tileButton->setToolTip(tileFile);
                
                // Añadir al grupo de botones
                tileButtons->addButton(tileButton, tileId);
                
                // Añadir a la cuadrícula
                tilesGridLayout->addWidget(tileButton, row, col % 3);
                
                // Actualizar posición para el siguiente tile
                col++;
                if (col % 3 == 0) {
                    row++;
                    col = 0;
                }
                
                tileId++;
            }
        }
    }
    
    // Conectar el grupo de botones a la selección de tiles
    connect(tileButtons, &QButtonGroup::idClicked,
            this, &MapEditor::tileSelected);
            
    qDebug() << "Se cargaron" << tileId << "tiles";
}

// Método para manejar la selección de tiles
void MapEditor::tileSelected(int id) {
    currentTileId = id;
    qDebug() << "Tile seleccionado:" << id;
}

// Método para colocar un tile en la posición del clic
void MapEditor::placeTile(QPointF scenePos) {
    // Solo proceder si hay un tile seleccionado
    if (currentTileId < 0 || !tilePixmaps.contains(currentTileId)) {
        return;
    }
    
    // Convertir la posición de la escena a coordenadas de cuadrícula
    QPoint gridPos = getTileGridPosition(scenePos);
    int gridX = gridPos.x();
    int gridY = gridPos.y();
    
    // Evitar colocar fuera de los límites
    if (gridX < 0 || gridX >= 31 || gridY < 0 || gridY >= 31) {
        return;
    }
    
    qDebug() << "Colocando tile" << currentTileId << "en posición de cuadrícula:" << gridX << "," << gridY;
    
    // Calcular la posición exacta en la escena
    qreal x = gridX * 32.0;
    qreal y = gridY * 32.0;
    
    // Crear un nuevo item gráfico con el tile seleccionado
    QGraphicsPixmapItem* tileItem = new QGraphicsPixmapItem(tilePixmaps[currentTileId]);
    tileItem->setPos(x, y);
    tileItem->setZValue(-0.5); // Por encima del fondo pero debajo de los elementos del mapa
    
    // Eliminar cualquier tile que pudiera haber en esa posición
    QPair<int, int> gridKey(gridX, gridY);
    for (QGraphicsItem* item : scene->items()) {
        QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        if (pixmapItem && pixmapItem->zValue() == -0.5) {
            QPoint itemPos = getTileGridPosition(pixmapItem->pos());
            if (itemPos == gridPos) {
                scene->removeItem(pixmapItem);
                delete pixmapItem;
                break;
            }
        }
    }
    
    // Añadir el nuevo tile a la escena
    scene->addItem(tileItem);
    
    // Guardar la referencia del tile colocado
    placedTiles[gridKey] = currentTileId;
}

// Método para eliminar un tile en la posición del clic derecho
void MapEditor::removeTile(QPointF scenePos) {
    // Convertir la posición de la escena a coordenadas de cuadrícula
    QPoint gridPos = getTileGridPosition(scenePos);
    int gridX = gridPos.x();
    int gridY = gridPos.y();
    
    // Verificar que esté dentro de los límites del mapa
    if (gridX < 0 || gridX >= 31 || gridY < 0 || gridY >= 31) {
        return;
    }
    
    // Clave para el mapa de tiles
    QPair<int, int> gridKey(gridX, gridY);
    
    // Verificar si hay un tile en esta posición
    if (placedTiles.contains(gridKey)) {
        // Eliminar la entrada del mapa de tiles
        placedTiles.remove(gridKey);
        
        // Buscar el item visual correspondiente y eliminarlo
        bool removed = false;
        for (QGraphicsItem* item : scene->items()) {
            QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item);
            if (pixmapItem && pixmapItem->zValue() == -0.5 && pixmapItem != currentBackground) {
                QPoint itemPos = getTileGridPosition(pixmapItem->pos());
                if (itemPos == gridPos) {
                    scene->removeItem(pixmapItem);
                    delete pixmapItem;
                    removed = true;
                    qDebug() << "Tile eliminado en posición" << gridX << "," << gridY;
                    break;
                }
            }
        }
        
        if (!removed) {
            qDebug() << "No se encontró el tile visual para eliminar en" << gridX << "," << gridY;
        }
    } else {
        qDebug() << "No hay tile para eliminar en la posición" << gridX << "," << gridY;
    }
}

// Convertir posición de la escena a coordenadas de la cuadrícula
QPoint MapEditor::getTileGridPosition(const QPointF& scenePos) {
    // Tamaño de cada celda de la cuadrícula (32x32 píxeles)
    int gridSize = 32;
    
    // Calcular las coordenadas de la cuadrícula
    int gridX = static_cast<int>(scenePos.x()) / gridSize;
    int gridY = static_cast<int>(scenePos.y()) / gridSize;
    
    // Retornar las coordenadas como un punto
    return QPoint(gridX, gridY);
}

// Método para actualizar la interfaz de usuario según el tipo de terreno
void MapEditor::updateTerrainUI(int terrainType)
{
    // Se podría usar para cambiar colores, estilos o comportamientos específicos
    // según el tipo de terreno seleccionado
    switch (terrainType) {
        case DESERT:
            setWindowTitle("Counter-Strike Editor - Mapa Desierto");
            break;
        case AZTEC_VILLAGE:
            setWindowTitle("Counter-Strike Editor - Mapa Pueblito Azteca");
            break;
        case TRAINING_ZONE:
            setWindowTitle("Counter-Strike Editor - Mapa Zona de Entrenamiento");
            break;
    }
}

void MapEditor::generarMapaClicked()
{
    // Validar que el mapa cumpla con los requisitos
    if (!validateMap()) {
        QMessageBox::warning(this, "Error de Validación", 
            "El mapa no cumple con los requisitos:\n" + 
            MapValidator::getValidationErrors().join("\n"));
        return;
    }
    
    QString nombreArchivo = obtenerNombreArchivo();
    if (!nombreArchivo.isEmpty()) {
        generateMapFile(nombreArchivo);
    }
}

QString MapEditor::obtenerNombreArchivo()
{
    bool ok;
    QString nombreArchivo = QInputDialog::getText(this, tr("Nombre del Mapa"), 
                                             tr("Ingrese el nombre del mapa:"), 
                                             QLineEdit::Normal, currentMapName, &ok);

    if (ok && !nombreArchivo.isEmpty()) {
        currentMapName = nombreArchivo;
        return nombreArchivo;
    } else {
        return QString();
    }
}

// Método para cargar un mapa
void MapEditor::loadMapClicked()
{
    // Verificar que exista el directorio server/maps
    QDir currentDir(QDir::current());
    QString serverDir = "../server/";
    QString mapsPath = serverDir + "maps/";
    
    qDebug() << "Buscando directorio para cargar mapas:" << currentDir.absolutePath();
    
    // Primero verificar que existe la carpeta server
    if (!currentDir.exists(serverDir)) {
        qDebug() << "La carpeta server no existe. Intentando crear carpeta server.";
        bool created = currentDir.mkdir(serverDir);
        qDebug() << "Creando directorio server:" << (created ? "Éxito" : "Fallo");
    }
    
    // Ahora intentar crear maps dentro de server si no existe
    if (!currentDir.exists(mapsPath)) {
        bool created = currentDir.mkpath(mapsPath); // mkpath crea directorios recursivamente si es necesario
        qDebug() << "Creando directorio server/maps:" << (created ? "Éxito" : "Fallo");
    }
    
    // Obtener la ruta absoluta para mostrar en el diálogo
    QString absoluteMapsPath = currentDir.absoluteFilePath(mapsPath);
    qDebug() << "Directorio de mapas para cargar:" << absoluteMapsPath;
    
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Abrir Mapa"), absoluteMapsPath, tr("Archivos YAML (*.yaml)"));
        
    if (!fileName.isEmpty()) {
        loadMapFromFile(fileName);
    }
}

// Método para cargar un mapa desde un archivo
void MapEditor::loadMapFromFile(const QString &fileName)
{
    // Limpiar la escena actual
    QList<QGraphicsItem*> itemsToRemove;
    foreach (QGraphicsItem *item, scene->items()) {
        if (auto* item_ptr = dynamic_cast<DragAndDrop*>(item)) {
            itemsToRemove.append(item);
        }
        // También eliminar tiles (son QGraphicsPixmapItem con zValue -0.5)
        else if (QGraphicsPixmapItem *pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item)) {
            if (pixmapItem->zValue() == -0.5 && pixmapItem != currentBackground) {
                itemsToRemove.append(item);
            }
        }
    }
    
    // Eliminar elementos manteniendo el fondo
    foreach (QGraphicsItem *item, itemsToRemove) {
        scene->removeItem(item);
        delete item;
    }
    
    // Limpiar el mapa de tiles colocados
    placedTiles.clear();
    
    QList<MapElement*> elements;
    int terrainType;
    QString mapName;
    
    // Cargar el mapa usando YamlHandler
    if (YamlHandler::loadMapFromYaml(fileName, elements, terrainType, mapName)) {
        // Actualizar el tipo de terreno
        currentTerrainType = terrainType;
        currentMapName = mapName;
        backgroundSelection(terrainType);
        
        // Crear elementos gráficos para cada elemento del mapa
        foreach (MapElement *element, elements) {
            // Si es un tile, procesarlo diferente
            if (element->getType() == TILE) {
                Tile *tile = dynamic_cast<Tile*>(element);
                if (tile && tile->getTileId() >= 0 && tilePixmaps.contains(tile->getTileId())) {
                    // Obtener la posición de la cuadrícula
                    QPoint gridPos = getTileGridPosition(tile->getPosition());
                    
                    // Colocar el tile en esa posición
                    QGraphicsPixmapItem* tileItem = new QGraphicsPixmapItem(tilePixmaps[tile->getTileId()]);
                    tileItem->setPos(gridPos.x() * 32, gridPos.y() * 32);
                    tileItem->setZValue(-0.5);
                    scene->addItem(tileItem);
                    
                    // Guardar la referencia del tile colocado
                    placedTiles[QPair<int, int>(gridPos.x(), gridPos.y())] = tile->getTileId();
                    
                    qDebug() << "Cargando tile" << tile->getTileId() << "en posición" << gridPos.x() << "," << gridPos.y();
                }
            } else {
                // Procesar otros elementos normalmente
                DragAndDrop *item = createDragAndDropItem(element);
                if (item) {
                    scene->addItem(item);
                }
            }
        }
        
        // Liberar memoria de los elementos cargados
        qDeleteAll(elements);
        elements.clear();
        
        QMessageBox::information(this, "Carga Exitosa", "El mapa ha sido cargado correctamente");
    } else {
        QMessageBox::critical(this, "Error", "No se pudo cargar el mapa");
    }
}

// Método para validar que el mapa cumpla con los requisitos
bool MapEditor::validateMap()
{
    QList<MapElement*> elements;
    
    // Convertir los elementos gráficos a elementos del mapa
    foreach (QGraphicsItem *item, scene->items()) {
        if (auto* item_ptr = dynamic_cast<DragAndDrop*>(item)) {
            MapElement *element = convertToMapElement(dragItem);
            if (element) {
                elements.append(element);
            }
        }
    }
    
    // Validar usando MapValidator
    bool isValid = MapValidator::validateMap(elements);
    
    // Liberar memoria
    qDeleteAll(elements);
    elements.clear();
    
    return isValid;
}

// Método para filtrar eventos, especialmente del ratón para colocar/eliminar tiles
bool MapEditor::eventFilter(QObject* watched, QEvent* event)
{
    // Solo procesamos eventos del viewport de la vista
    if (watched != view->viewport()) {
        return false;
    }
    
    // Manejar eventos de ratón
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QPointF scenePos = view->mapToScene(mouseEvent->pos());
        
        // Verificar que la posición esté dentro de los límites del mapa
        if (scenePos.x() < 0 || scenePos.y() < 0 || 
            scenePos.x() >= scene->width() || scenePos.y() >= scene->height()) {
            return false;
        }
        
        // Clic izquierdo: colocar tile seleccionado
        if (mouseEvent->button() == Qt::LeftButton) {
            if (currentTileId >= 0) { // Solo si hay un tile seleccionado
                placeTile(scenePos);
                return true; // Evento procesado
            }
        }
        // Clic derecho: eliminar tile existente
        else if (mouseEvent->button() == Qt::RightButton) {
            removeTile(scenePos);
            return true; // Evento procesado
        }
    }
    
    // Dejar que Qt maneje otros eventos
    return false;
}

void MapEditor::generateMapFile(const QString &fileName)
{
    // Asegurar que exista el directorio de mapas dentro de server
    QString serverDir = "../server/";
    QString mapsDir = serverDir + "maps/";
    QDir dir(QDir::current());
    
    qDebug() << "Directorio actual para guardar mapas:" << dir.absolutePath();
    
    // Primero verificar que existe la carpeta server
    if (!dir.exists(serverDir)) {
        qDebug() << "La carpeta server no existe. Intentando crear carpeta server.";
        bool created = dir.mkdir(serverDir);
        qDebug() << "Creando directorio server:" << (created ? "Éxito" : "Fallo");
    }
    
    // Ahora intentar crear maps dentro de server
    if (!dir.exists(mapsDir)) {
        bool created = dir.mkpath(mapsDir); // mkpath crea directorios recursivamente si es necesario
        qDebug() << "Creando directorio server/maps:" << (created ? "Éxito" : "Fallo");
    } else {
        qDebug() << "Directorio server/maps ya existe en:" << dir.absoluteFilePath(mapsDir);
    }
    
    // Actualizar el directorio donde se guardarán los mapas
    QString directorio = mapsDir;

    // Lista para almacenar todos los elementos
    QList<MapElement*> elements;
    
    // Convertir todos los elementos gráficos a elementos del mapa
    foreach (QGraphicsItem *item, scene->items()) {
        if (auto* item_ptr = dynamic_cast<DragAndDrop*>(item)) {
            MapElement *element = convertToMapElement(dragItem);
            if (element) {
                elements.append(element);
            }
        }
    }
    
    // Añadir los tiles colocados como elementos del mapa
    QMap<QPair<int, int>, int>::const_iterator it;
    for (it = placedTiles.constBegin(); it != placedTiles.constEnd(); ++it) {
        QPair<int, int> gridPos = it.key();
        int tileId = it.value();
        
        // Posición en píxeles (cada celda es 32x32)
        QPointF pixelPos(gridPos.first * 32.0, gridPos.second * 32.0);
        
        // Crear un elemento Tile
        MapElement *tileElement = new Tile(pixelPos, tileId);
        elements.append(tileElement);
        
        qDebug() << "Guardando tile" << tileId << "en posición" << gridPos.first << "," << gridPos.second;
    }

    // Guardar el mapa en formato YAML
    bool success = YamlHandler::saveMapToYaml(
        directorio + fileName + ".yaml",
        elements,
        currentTerrainType,
        fileName
    );

    // Liberar memoria
    qDeleteAll(elements);
    elements.clear();

    if (success) {
        QMessageBox::information(this, "Guardado Exitoso", 
            "El mapa ha sido guardado correctamente en " + directorio + fileName + ".yaml");
    } else {
        QMessageBox::critical(this, "Error", 
            "No se pudo guardar el mapa en " + directorio + fileName + ".yaml");
    }
}

// Convertir un elemento gráfico (DragAndDrop) a un elemento del mapa (MapElement)
MapElement* MapEditor::convertToMapElement(DragAndDrop* item)
{
    if (!item) return nullptr;
    
    QPointF position = item->scenePos();
    int elementType = item->getTipoElemento();
    
    // Convertir posición de pixeles a unidades de mundo si es necesario
    float elementWidth = (item->pixmap().width()) / 25.6;
    float elementHeight = (item->pixmap().height()) / 25.6;
    float x = MapEditor::pixelToWorldX(position.x(), elementWidth);
    float y = MapEditor::pixelToWorldY(position.y(), elementHeight);
    
    QPointF worldPos(x, y);
    
    switch (elementType) {
        case TEAM_SPAWN_CT:
        case TEAM_SPAWN_T: {
            int teamId = item->getTeamId();
            return new TeamSpawn(worldPos, teamId);
        }
        
        case BOMB_ZONE: {
            QSizeF size = item->getBombZoneSize();
            return new BombZone(worldPos, size);
        }
        
        case SOLID_STRUCTURE: {
            int structType = item->getStructureType();
            return new SolidStructure(worldPos, structType);
        }
        
        case WEAPON: {
            int weaponType = item->getWeaponType();
            return new Weapon(worldPos, weaponType);
        }
        
        default:
            return nullptr;
    }
}

// Crear un elemento gráfico (DragAndDrop) a partir de un elemento del mapa (MapElement)
DragAndDrop* MapEditor::createDragAndDropItem(const MapElement* element)
{
    if (!element) return nullptr;
    
    int elementType = element->getType();
    QPointF position = element->getPosition();
    
    // Convertir de unidades de mundo a pixeles si es necesario
    float x = MapEditor::worldToPixelX(position.x());
    float y = MapEditor::worldToPixelY(position.y());
    
    QPointF pixelPos(x, y);
    
    // Crear el elemento gráfico según su tipo
    QPixmap pixmap;
    DragAndDrop* item = nullptr;
    
    switch (elementType) {
        case TEAM_SPAWN_CT:
        case TEAM_SPAWN_T: {
            const TeamSpawn* spawn = static_cast<const TeamSpawn*>(element);
            pixmap = QPixmap(getResourcePath(elementType));
            item = new DragAndDrop(pixmap, 0.8, scene);
            item->setTipoElemento(elementType);
            item->setTeamId(spawn->getTeamId());
            break;
        }
        
        case BOMB_ZONE: {
            const BombZone* zone = static_cast<const BombZone*>(element);
            pixmap = QPixmap(getResourcePath(BOMB_ZONE));
            item = new DragAndDrop(pixmap, 1.0, scene);
            item->setTipoElemento(BOMB_ZONE);
            item->setBombZoneSize(zone->getSize());
            break;
        }
        
        case SOLID_STRUCTURE: {
            const SolidStructure* structure = static_cast<const SolidStructure*>(element);
            int structType = structure->getStructureType();
            pixmap = QPixmap(getResourcePath(SOLID_STRUCTURE, structType));
            item = new DragAndDrop(pixmap, 1.0, scene);
            item->setTipoElemento(SOLID_STRUCTURE);
            item->setStructureType(structType);
            break;
        }
        
        case WEAPON: {
            const Weapon* weapon = static_cast<const Weapon*>(element);
            int weaponType = weapon->getWeaponType();
            pixmap = QPixmap(getResourcePath(WEAPON, weaponType));
            item = new DragAndDrop(pixmap, 0.6, scene);
            item->setTipoElemento(WEAPON);
            item->setWeaponType(weaponType);
            break;
        }
    }
    
    if (item) {
        item->setPos(pixelPos);
    }
    
    return item;
}
