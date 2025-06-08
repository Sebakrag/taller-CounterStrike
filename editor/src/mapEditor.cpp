#include "mapEditor.h"
#include "dragAndDrop.h"
#include "backgroundItem.h"
#include <iostream>
#include <QScrollBar>
#include <QCoreApplication>
#include <QPainter>
#include <QImage>

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
    
    // Alternativa: intentar con una ruta relativa desde el directorio actual
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

// Método para obtener la ruta a un recurso según el tipo de elemento
QString MapEditor::getResourcePath(int elementType, int subType)
{
    QString basePath = getResourcesPath();
    
    switch (elementType) {
        case TEAM_SPAWN_CT:
            return basePath + "player/ct_spawn.png";
            
        case TEAM_SPAWN_T:
            return basePath + "player/t_spawn.png";
            
        case BOMB_ZONE:
            return basePath + "objects/bomb_zone.png";
            
        case SOLID_STRUCTURE:
            // Diferentes tipos de estructuras según el terreno seleccionado
            switch (currentTerrainType) {
                case DESERT: 
                    return basePath + "objects/desert_box0.png";
                case AZTEC_VILLAGE: 
                    return basePath + "objects/aztec_box0.png";
                case TRAINING_ZONE: 
                    return basePath + "objects/train_box0.png";
                default:
                    return basePath + "objects/box.png";
            }
            
        case WEAPON:
            switch (subType) {
                case PISTOL:
                    return basePath + "weapons/pistol.png";
                case RIFLE:
                    return basePath + "weapons/rifle.png";
                case SNIPER:
                    return basePath + "weapons/sniper.png";
                case SHOTGUN:
                    return basePath + "weapons/shotgun.png";
                default:
                    return basePath + "weapons/pistol.png";
            }
            
        default:
            // Imagen genérica para tipos desconocidos
            return basePath + "objects/default.png";
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
        if (dynamic_cast<DragAndDrop*>(item)) {
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
