#include "mapEditor.h"
#include "dragAndDrop.h"
#include "backgroundItem.h"
#include <iostream>
#include <QScrollBar>

MapEditor::MapEditor(QWidget *parent) : QMainWindow(parent), currentBackground(nullptr), currentTerrainType(0)
{
    // Configurar una escena más grande para scrolling
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1024, 1024); // Tamaño mayor para soportar scrolling

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

    // Crear la interfaz para agregar objetos
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
    
    // Añadir todas las secciones al panel de herramientas
    toolLayout->addWidget(terrainGroup);
    toolLayout->addWidget(teamsGroup);
    toolLayout->addWidget(objectsGroup);
    toolLayout->addWidget(weaponsGroup);
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
    QPixmap backgroundPixmap;
    currentTerrainType = index; // Guardar el tipo de terreno seleccionado
    QString basePath = "../gfx/backgrounds/";

    switch (index) {
        case DESERT: // Desierto
            backgroundPixmap = QPixmap(basePath + "dust2.bmp");
            break;
        case AZTEC_VILLAGE: // Pueblito Azteca
            backgroundPixmap = QPixmap(basePath + "aztec.bmp");
            break;
        case TRAINING_ZONE: // Zona de entrenamiento
            backgroundPixmap = QPixmap(basePath + "train.bmp");
            break;
        default:
            backgroundPixmap = QPixmap(basePath + "dust2.bmp"); // Por defecto desierto
            break;
    }

    // Si la imagen está vacía, intentar con un fondo genérico
    if (backgroundPixmap.isNull()) {
        qDebug() << "No se pudo cargar la imagen de fondo, usando fondo genérico.";
        backgroundPixmap = QPixmap(32, 32);
        backgroundPixmap.fill(getTerrainColor(index));
    }

    // Escalar la imagen al tamaño de la escena si es necesario
    QRectF sceneRect = scene->sceneRect();
    if (!backgroundPixmap.isNull() && 
        (backgroundPixmap.width() < sceneRect.width() || 
         backgroundPixmap.height() < sceneRect.height())) {
        backgroundPixmap = backgroundPixmap.scaled(
            sceneRect.width(), sceneRect.height(),
            Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation
        );
    }

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

// Método para obtener la ruta a un recurso según el tipo de elemento
QString MapEditor::getResourcePath(int elementType, int subType)
{
    QString basePath = "../gfx/";
    
    switch (elementType) {
        case TEAM_SPAWN_CT:
            return basePath + "player/ct_spec.bmp";
            
        case TEAM_SPAWN_T:
            return basePath + "player/t_spec.bmp";
            
        case BOMB_ZONE:
            return basePath + "weapons/c4.bmp";
            
        case SOLID_STRUCTURE:
            // Diferentes tipos de estructuras según el terreno seleccionado
            switch (currentTerrainType) {
                case DESERT: {
                    // Usar tiles específicos de zonas desérticas
                    int tileId = subType % 4; // Limitar a 4 tipos de tiles diferentes
                    QStringList desertTiles = {
                        "tiles/mideast/wood_box.bmp", 
                        "tiles/mideast/block.bmp",
                        "tiles/mideast/barrel.bmp",
                        "tiles/mideast/pot.bmp"
                    };
                    return basePath + desertTiles[tileId];
                }
                case AZTEC_VILLAGE: {
                    // Usar tiles específicos de zonas aztecas
                    int tileId = subType % 4; // Limitar a 4 tipos de tiles diferentes
                    QStringList aztecTiles = {
                        "tiles/aztec/aztec_box.bmp", 
                        "tiles/aztec/aztec_block.bmp",
                        "tiles/aztec/aztec_barrel.bmp",
                        "tiles/aztec/aztec_pot.bmp"
                    };
                    return basePath + aztecTiles[tileId];
                }
                case TRAINING_ZONE: {
                    // Usar tiles específicos de zonas de entrenamiento
                    int tileId = subType % 4; // Limitar a 4 tipos de tiles diferentes
                    QStringList trainingTiles = {
                        "tiles/training/box.bmp", 
                        "tiles/training/block.bmp",
                        "tiles/training/barrel.bmp",
                        "tiles/training/wall.bmp"
                    };
                    return basePath + trainingTiles[tileId];
                }
                default:
                    return basePath + "tiles/box.bmp";
            }
            
        case WEAPON:
            switch (subType) {
                case PISTOL:
                    return basePath + "weapons/usp.bmp";
                case RIFLE:
                    return basePath + "weapons/m4a1.bmp";
                case SNIPER:
                    return basePath + "weapons/awp.bmp";
                case SHOTGUN:
                    return basePath + "weapons/xm1014.bmp";
                default:
                    return basePath + "weapons/glock.bmp";
            }
            
        default:
            // Imagen genérica para tipos desconocidos
            return basePath + "tiles/box.bmp";
    }
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
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Abrir Mapa"), "maps/", tr("Archivos YAML (*.yaml)"));
        
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
        if (DragAndDrop *dragItem = dynamic_cast<DragAndDrop*>(item)) {
            itemsToRemove.append(item);
        }
    }
    
    // Eliminar elementos manteniendo el fondo
    foreach (QGraphicsItem *item, itemsToRemove) {
        scene->removeItem(item);
        delete item;
    }
    
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
            DragAndDrop *item = createDragAndDropItem(element);
            if (item) {
                scene->addItem(item);
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
        if (DragAndDrop *dragItem = dynamic_cast<DragAndDrop*>(item)) {
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

void MapEditor::generateMapFile(const QString &fileName)
{
    // Asegurar que exista el directorio de mapas
    QString directorio = "maps/";
    QDir dir;
    if (!dir.exists(directorio)) {
        dir.mkdir(directorio);
    }

    // Lista para almacenar todos los elementos
    QList<MapElement*> elements;
    
    // Convertir todos los elementos gráficos a elementos del mapa
    foreach (QGraphicsItem *item, scene->items()) {
        if (DragAndDrop *dragItem = dynamic_cast<DragAndDrop*>(item)) {
            MapElement *element = convertToMapElement(dragItem);
            if (element) {
                elements.append(element);
            }
        }
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
    float x = WorldScale::pixelToWorldX(position.x(), elementWidth);
    float y = WorldScale::pixelToWorldY(position.y(), elementHeight);
    
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
    float x = WorldScale::worldToPixelX(position.x());
    float y = WorldScale::worldToPixelY(position.y());
    
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
