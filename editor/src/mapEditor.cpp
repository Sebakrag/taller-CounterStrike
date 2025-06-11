#include "mapEditor.h"
#include "dragAndDrop.h"
#include "backgroundItem.h"
#include "tileItem.h"
#include <iostream>
#include <QScrollBar>
#include <QCoreApplication>
#include <QPainter>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QDir>

MapEditor::MapEditor(QWidget *parent) : QMainWindow(parent), currentBackground(nullptr),
    currentTerrainType(0)
    // Nota: Estas variables ya están inicializadas en la declaración de la clase (mapEditor.h)
{
    // Configurar una escena de 1000x1000 para el mapa de tiles
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1000, 1000); // Tamaño exacto para mapa de 1000x1000
    
    // Inicializar los button groups
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

    // Panel derecho para controles y selección de elementos del mapa
    toolPanel = new QWidget();
    
    // Grupo para opciones de terreno
    QGroupBox* terrainGroup = new QGroupBox("Tipo de Terreno");
    QVBoxLayout* terrainLayout = new QVBoxLayout(terrainGroup);
    
    // Opciones de terreno
    terrainCombo = new QComboBox();
    terrainCombo->addItem("Desert");
    terrainCombo->addItem("Aztec");
    terrainCombo->addItem("Training Zone");
    terrainLayout->addWidget(terrainCombo);
    
    // Crear pestañas para diferentes tipos de elementos
    elementsTabWidget = new QTabWidget();
    
    // Pestaña para Tiles
    QWidget* tilesTab = new QWidget();
    QVBoxLayout* tilesTabLayout = new QVBoxLayout(tilesTab);
    
    // Preparar el área de selección de tiles
    tilesGroup = new QGroupBox("Tiles");
    QVBoxLayout* tilesLayout = new QVBoxLayout(tilesGroup);
    
    tilesScrollArea = new QScrollArea();
    tilesScrollArea->setWidgetResizable(true);
    QWidget* tilesContainer = new QWidget();
    new QGridLayout(tilesContainer);
    tilesScrollArea->setWidget(tilesContainer);
    tilesLayout->addWidget(tilesScrollArea);
    
    // Grupo de botones para tiles
    tileButtons = new QButtonGroup(this);
    
    tilesTabLayout->addWidget(tilesGroup);
    elementsTabWidget->addTab(tilesTab, "Tiles");
    
    // Pestaña para elementos sólidos
    QWidget* solidsTab = new QWidget();
    QVBoxLayout* solidsTabLayout = new QVBoxLayout(solidsTab);
    
    // Área de desplazamiento para sólidos
    solidsGroup = new QGroupBox("Sólidos");
    QVBoxLayout* solidsLayout = new QVBoxLayout(solidsGroup);
    
    solidsScrollArea = new QScrollArea();
    solidsScrollArea->setWidgetResizable(true);
    QWidget* solidsContainer = new QWidget();
    new QGridLayout(solidsContainer);
    solidsScrollArea->setWidget(solidsContainer);
    solidsLayout->addWidget(solidsScrollArea);
    
    // Grupo de botones para sólidos
    solidButtons = new QButtonGroup(this);
    
    solidsTabLayout->addWidget(solidsGroup);
    elementsTabWidget->addTab(solidsTab, "Sólidos");
    
    // Pestaña para zonas
    QWidget* zonesTab = new QWidget();
    QVBoxLayout* zonesTabLayout = new QVBoxLayout(zonesTab);
    
    // Área de desplazamiento para zonas
    zonesGroup = new QGroupBox("Zonas");
    QVBoxLayout* zonesLayout = new QVBoxLayout(zonesGroup);
    
    zonesScrollArea = new QScrollArea();
    zonesScrollArea->setWidgetResizable(true);
    QWidget* zonesContainer = new QWidget();
    new QGridLayout(zonesContainer);
    zonesScrollArea->setWidget(zonesContainer);
    zonesLayout->addWidget(zonesScrollArea);
    
    // Grupo de botones para zonas
    zoneButtons = new QButtonGroup(this);
    
    zonesTabLayout->addWidget(zonesGroup);
    elementsTabWidget->addTab(zonesTab, "Zonas");
    
    // Pestaña para armas
    QWidget* weaponsTab = new QWidget();
    QVBoxLayout* weaponsTabLayout = new QVBoxLayout(weaponsTab);
    
    // Área de desplazamiento para armas
    weaponsGroup = new QGroupBox("Armas");
    QVBoxLayout* weaponsLayout = new QVBoxLayout(weaponsGroup);
    
    weaponsScrollArea = new QScrollArea();
    weaponsScrollArea->setWidgetResizable(true);
    QWidget* weaponsContainer = new QWidget();
    new QGridLayout(weaponsContainer);
    weaponsScrollArea->setWidget(weaponsContainer);
    weaponsLayout->addWidget(weaponsScrollArea);
    
    // Grupo de botones para armas
    weaponButtons = new QButtonGroup(this);
    
    // Añadir la pestaña de armas a las pestañas
    weaponsTabLayout->addWidget(weaponsGroup);
    elementsTabWidget->addTab(weaponsTab, "Armas");
    
    // Layout principal para elementos en groupBox
    toolPanelLayout = new QVBoxLayout(toolPanel);
    toolPanelLayout->setContentsMargins(0, 0, 0, 0);
    toolPanelLayout->setSpacing(5);
    toolPanelLayout->addWidget(terrainGroup);
    toolPanelLayout->addWidget(elementsTabWidget);
    
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
    
    // Configurar el área de selección de tiles que ya se declaró anteriormente
    tilesScrollArea->setMinimumHeight(200);
    tilesScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    // Añadir secciones restantes al panel de herramientas
    // Usamos el toolPanelLayout ya creado anteriormente 
    if (toolPanelLayout) { // Si existe, añadimos los widgets restantes
        toolPanelLayout->addWidget(zoomGroup);
        toolPanelLayout->addWidget(fileGroup);
        toolPanelLayout->addStretch(1); // Espacio flexible al final
    }
    
    // Conectar señales
    connect(terrainCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MapEditor::backgroundSelection);
    
    // Conectar botones de carga y guardado
    connect(loadMapButton, &QPushButton::clicked, this, &MapEditor::loadMapClicked);
    connect(saveMapButton, &QPushButton::clicked, this, &MapEditor::generarMapaClicked);
    
    // Conectar botones de zoom
    connect(zoomInButton, &QPushButton::clicked, this, &MapEditor::zoomIn);
    connect(zoomOutButton, &QPushButton::clicked, this, &MapEditor::zoomOut);
    
    // Instalamos un event filter en la vista para capturar clicks y movimientos
    view->viewport()->installEventFilter(this);
    
    // No necesitamos un indicador en la cuadrícula porque resaltaremos los elementos en el menú
    
    // Estos métodos se llamarán más abajo en backgroundSelection(0)
    
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
    
    try {
        // Inicializar con el fondo de desierto por defecto - usar try-catch para identificar donde está el segfault
        qDebug() << "A punto de llamar a backgroundSelection(0)";
        backgroundSelection(0); // Carga los tiles disponibles y configura el fondo
        qDebug() << "backgroundSelection completado con éxito";
    } catch (const std::exception& e) {
        qCritical() << "Excepción en el constructor de MapEditor:" << e.what();
    } catch (...) {
        qCritical() << "Excepción desconocida en el constructor de MapEditor";
    }
    
    // Añadir mensaje final para confirmar que el constructor se completó
    qDebug() << "Constructor de MapEditor finalizado";
}

// Implementación del destructor
// Método para actualizar el estilo del botón seleccionado
void MapEditor::updateSelectedButtonStyle(QButtonGroup* buttonGroup, int selectedId)
{
    // Estilo para resaltar el botón seleccionado
    const QString selectedStyle = "QPushButton { border: 2px solid #FFD700; background-color: rgba(255, 215, 0, 0.3); }";
    const QString normalStyle = "";
    
    // Recorrer todos los botones y actualizar su estilo
    for (QAbstractButton* button : buttonGroup->buttons()) {
        int id = buttonGroup->id(button);
        if (id == selectedId) {
            // Aplicar estilo seleccionado
            button->setStyleSheet(selectedStyle);
        } else {
            // Restaurar estilo normal
            button->setStyleSheet(normalStyle);
        }
    }
}

MapEditor::~MapEditor()
{
    // Desconectar todas las señales para evitar que se activen durante la destrucción
    disconnect(this); // Desconecta todas las conexiones que involucran a esta instancia
    
    // Primero intentar remover elementos de la escena para evitar accesos inválidos
    if (scene) {
        scene->clear(); // Limpia todos los items de la escena
    }
    
    // Limpiar primero currentBackground para evitar accesos inválidos
    if (currentBackground) {
        currentBackground = nullptr; // Evita acceso a memoria liberada
    }
    
    // Limpiar los mapas de tiles
    tilePixmaps.clear();
    placedTiles.clear();
}

// Método eliminado - ahora los elementos se colocan directamente con el clic

// Método eliminado - ahora los elementos se colocan directamente con el clic

// Método eliminado - ahora los elementos se colocan directamente con el clic
// Método eliminado - ahora los elementos se colocan directamente con el clic

void MapEditor::backgroundSelection(int index)
{
    // Proteger contra llamadas recursivas
    static bool isUpdating = false;
    if (isUpdating) return;
    isUpdating = true;

    // Borrar el fondo anterior si existe
    if (currentBackground) {
        scene->removeItem(currentBackground);
        delete currentBackground;
        currentBackground = nullptr;
    }
    
    // Eliminar todos los tiles que pudieran existir en la escena
    QList<QGraphicsItem*> tilesToRemove;
    for (QGraphicsItem* item : scene->items()) {
        QGraphicsPixmapItem* tileItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        if (tileItem && tileItem->data(1).toInt() == TILE) {
            tilesToRemove.append(tileItem);
        }
    }
    
    // Eliminar los tiles encontrados
    for (QGraphicsItem* item : tilesToRemove) {
        scene->removeItem(item);
        delete item;
    }
    
    // Limpiar el registro de tiles colocados
    placedTiles.clear();
    
    // Actualizar indice de terreno
    currentTerrainType = index;
    
    // Crear nueva cuadrícula de fondo de 1000x1000 con celdas de 32x32 píxeles
    currentBackground = new BackgroundItem(1000, 1000, 32);
    currentBackground->setPos(0, 0);
    scene->addItem(currentBackground);
    currentBackground->setZValue(-1000); // Asegurarse de que el fondo esté detrás de todo
    
    // También eliminar todos los elementos del mapa ya que estamos cambiando el entorno
    for (QGraphicsItem* item : scene->items()) {
        // Asegurarse de no eliminar el fondo actual
        if (item != currentBackground) {
            DragAndDrop* dragItem = dynamic_cast<DragAndDrop*>(item);
            if (dragItem) {
                scene->removeItem(dragItem);
                delete dragItem;
            }
        }
    }
    
    // Asegurar que las barras de desplazamiento estén habilitadas
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    // Cargar nuevos tiles según el terreno seleccionado
    // Pero evitar la recursion infinita que ocurre porque loadAvailableTiles llama a este método indirectamente
    loadAvailableTiles();
    
    // Actualizar la interfaz según el terreno
    updateTerrainUI(index);
    
    isUpdating = false;
}

// Método para aumentar el zoom
void MapEditor::zoomIn() {
    view->scale(1.2, 1.2);
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
    QString projectDir = QDir::currentPath();
    
    // Imprimimos el directorio actual para debug
    qDebug() << "Directorio actual:" << projectDir;
    
    // Opciones de rutas relativas desde diferentes puntos de compilación
    QStringList possiblePaths = {
        "/Users/morenasandroni/Facultad/Taller/2025c1/taller-CounterStrike/editor/resources/",
        "../editor/resources/",
        "resources/",
        "../resources/",
        "../../editor/resources/",
        QCoreApplication::applicationDirPath() + "/resources/",
        QCoreApplication::applicationDirPath() + "/../editor/resources/"
    };
    
    // Buscar la primera ruta válida
    for (const QString& path : possiblePaths) {
        if (QDir(path).exists()) {
            qDebug() << "Encontrada carpeta de recursos en:" << path;
            return path;
        }
    }
    
    // Si ninguna de las rutas anteriores funciona, intentamos encontrar la ruta en relación 
    // al directorio de compilación
    QString buildDir = QDir::currentPath();
    QString targetPath = buildDir + "/../editor/resources/";
    if (QDir(targetPath).exists()) {
        qDebug() << "Usando ruta de recursos relativa a build:" << targetPath;
        return targetPath;
    }
    
    qWarning() << "No se encontró la carpeta de recursos. Usando ruta predeterminada.";
    return "../editor/resources/";
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
            return basePath + "unknown.png";
    }
}

// Método genérico para cargar elementos desde una carpeta
void MapEditor::loadElementsFromPath(const QString& path, QMap<int, QPixmap>& pixmapMap,
                                   QButtonGroup* buttonGroup, QScrollArea* scrollArea,
                                   void (MapEditor::*selectCallback)(int))
{
    try {
        // Validación de parámetros para evitar segmentation fault
        if (!buttonGroup || !scrollArea) {
            qCritical() << "Error: buttonGroup o scrollArea son nullptr en loadElementsFromPath";
            return;
        }
        
        // Clear previous elements
        pixmapMap.clear();
        QDir elementsDir(path);
        
        // Si la carpeta no existe, solo registramos el mensaje pero continuamos sin error fatal
        if (!elementsDir.exists()) {
            qWarning() << "No se encontró la carpeta de elementos:" << path << ". Agregue archivos .png o .bmp para verlos aquí.";
            // Nótese que NO retornamos, permitimos que el método continúe para configurar los widgets incluso sin archivos
        }
        
        // Filtrar por archivos de imagen
        QStringList filters;
        filters << "*.bmp" << "*.png";
        elementsDir.setNameFilters(filters);
        
        // Obtener la lista de archivos
        QStringList elementFiles = elementsDir.entryList(filters, QDir::Files);
        qDebug() << "Elementos encontrados:" << elementFiles.count() << "en" << path;
        
        // Validación y creación de widgets si es necesario
        QWidget* elementsContainer = scrollArea->widget();
        if (!elementsContainer) {
            qWarning() << "ScrollArea sin widget contenedor. Creando uno nuevo.";
            elementsContainer = new QWidget();
            scrollArea->setWidget(elementsContainer);
            scrollArea->setWidgetResizable(true);
        }
        
        QGridLayout* elementsGridLayout = qobject_cast<QGridLayout*>(elementsContainer->layout());
        if (!elementsGridLayout) {
            qDebug() << "Creando nuevo layout para el contenedor de elementos";
            if (elementsContainer->layout()) {
                delete elementsContainer->layout();
            }
            elementsGridLayout = new QGridLayout(elementsContainer);
            elementsGridLayout->setSpacing(2);
            elementsGridLayout->setContentsMargins(2, 2, 2, 2);
        }
        
        // Eliminar botones existentes
        QList<QAbstractButton*> buttons = buttonGroup->buttons();
        for (QAbstractButton* button : buttons) {
            buttonGroup->removeButton(button);
            delete button;
        }
        
        // Desconectar conexiones anteriores para evitar conexiones duplicadas
        disconnect(buttonGroup, nullptr, this, nullptr);

        // Conectar el grupo de botones al callback de selección
        if (selectCallback) {
            connect(buttonGroup, &QButtonGroup::idClicked,
                    this, selectCallback);
        }
        
        // Cargar cada archivo de elemento
        int elementId = 0;
        int row = 0;
        int col = 0;
        
        for (const QString& elementFile : elementFiles) {
            // Solo procesar archivos de imagen
            if (elementFile.endsWith(".bmp", Qt::CaseInsensitive) || 
                elementFile.endsWith(".png", Qt::CaseInsensitive)) {
                
                QString elementPath = path + elementFile;
                QPixmap elementPixmap(elementPath);
                
                if (!elementPixmap.isNull()) {
                    // Almacenar el pixmap
                    pixmapMap[elementId] = elementPixmap;
                    
                    // Crear un botón con una miniatura del elemento
                    QPushButton* elementButton = new QPushButton();
                    elementButton->setFixedSize(40, 40);
                    
                    // Crear una versión escalada para el botón
                    QPixmap scaledPixmap = elementPixmap.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    QIcon buttonIcon(scaledPixmap);
                    elementButton->setIcon(buttonIcon);
                    elementButton->setIconSize(QSize(36, 36));
                    elementButton->setToolTip(elementFile);
                    elementButton->setStyleSheet(""); // Inicializar sin estilo especial
                    
                    // Añadir al grupo de botones
                    buttonGroup->addButton(elementButton, elementId);
                    
                    // Añadir a la cuadrícula
                    elementsGridLayout->addWidget(elementButton, row, col % 3);
                    
                    // Actualizar posición para el siguiente elemento
                    col++;
                    if (col % 3 == 0) {
                        row++;
                        col = 0;
                    }
                    
                    elementId++;
                }
            }
        }
        
        qDebug() << "Se cargaron" << elementId << "elementos";
    } catch (const std::exception& e) {
        qCritical() << "Excepción en loadElementsFromPath:" << e.what();
    } catch (...) {
        qCritical() << "Excepción desconocida en loadElementsFromPath";
    }
}

void MapEditor::loadAvailableTiles()
{
    try {
        QString resourceBasePath = getResourcesPath();
        qDebug() << "Usando ruta de recursos base:" << resourceBasePath;
        
        // Crear la estructura de carpetas básica con privilegios adecuados
        // Intentamos crear primero solo la carpeta base de recursos
        QDir baseDir;
        if (!QDir(resourceBasePath).exists()) {
            bool created = baseDir.mkpath(resourceBasePath);
            qDebug() << "Intento de crear carpeta base:" << resourceBasePath << ", resultado:" << created;
            
            if (!created) {
                qWarning() << "No se puede crear la carpeta de recursos base. Usar una ubicación alternativa.";
                // Intentar con una ubicación alternativa - directorio actual
                resourceBasePath = QDir::currentPath() + "/editor_resources/";
                created = baseDir.mkpath(resourceBasePath);
                qDebug() << "Intento alternativo en:" << resourceBasePath << ", resultado:" << created;
                
                if (!created) {
                    qWarning() << "No se pueden crear carpetas de recursos. El editor funcionará con limitaciones.";
                }
            }
        }
        
        // Definir la estructura de carpetas requerida
        QStringList requiredDirs = {
            resourceBasePath + "tiles/",
            resourceBasePath + "solid/",
            resourceBasePath + "zones/",
            resourceBasePath + "weapons/"
        };
        
        // Crear subcarpetas de terrenos
        QStringList terrainTypes = {"desert", "aztec", "training"};
        for (const auto& terrain : terrainTypes) {
            requiredDirs << resourceBasePath + "tiles/tiles_" + terrain + "/";
        }
        
        // Crear todas las carpetas necesarias
        bool allCreated = true;
        for (const auto& dir : requiredDirs) {
            if (!QDir(dir).exists()) {
                bool created = QDir().mkpath(dir);
                qDebug() << "Creando directorio:" << dir << ", resultado:" << created;
                allCreated = allCreated && created;
            }
        }
        
        if (!allCreated) {
            qWarning() << "No se pudieron crear todas las carpetas de recursos. El editor funcionará con limitaciones.";
        } else {
            qDebug() << "Estructura de directorios creada correctamente.";
        }
        
        // Cargar los recursos disponibles según el tipo de terreno actual
        QString terrainFolder;
        switch (currentTerrainType) {
            case 0: // DESERT_TERRAIN
                terrainFolder = "desert";
                break;
            case 1: // AZTEC_VILLAGE
                terrainFolder = "aztec";
                break;
            case 2: // TRAINING_GROUND
                terrainFolder = "training";
                break;
            default:
                terrainFolder = "desert";
                break;
        }
        
        QString tilesPath = resourceBasePath + "tiles/tiles_" + terrainFolder + "/";
        
        // Comprobar que los containers y scroll areas existen antes de usarlos
        if (!tilesScrollArea) {
            qWarning() << "tilesScrollArea es nullptr! Inicializando...";
            tilesScrollArea = new QScrollArea();
            tilesScrollArea->setWidgetResizable(true);
            QWidget* tilesContainer = new QWidget();
            tilesContainer->setLayout(new QGridLayout());
            tilesScrollArea->setWidget(tilesContainer);
        }
        
        if (!tileButtons) {
            qWarning() << "tileButtons es nullptr! Inicializando...";
            tileButtons = new QButtonGroup(this);
        }
        
        loadElementsFromPath(tilesPath, tilePixmaps, tileButtons, tilesScrollArea, &MapEditor::tileSelected);
        
        // Verificar todos los punteros antes de cargar otros elementos
        if (!solidsScrollArea || !solidButtons || !zonesScrollArea || !zoneButtons 
            || !weaponsScrollArea || !weaponButtons) {
            qWarning() << "Algunos punteros son nullptr! No se cargarán más recursos para evitar fallos.";
            return;
        }
        
        // También cargar otros tipos de elementos
        loadAvailableSolids();
        loadAvailableZones();
        loadAvailableWeapons();
    } catch (const std::exception& e) {
        qCritical() << "Excepción capturada en loadAvailableTiles:" << e.what();
    } catch (...) {
        qCritical() << "Excepción desconocida capturada en loadAvailableTiles";
    }
}

void MapEditor::loadAvailableSolids() {
    QString solidsPath = getResourcesPath() + "solid/";
    loadElementsFromPath(solidsPath, solidPixmaps, solidButtons, solidsScrollArea, &MapEditor::solidSelected);
}

void MapEditor::loadAvailableZones() {
    QString zonesPath = getResourcesPath() + "zones/";
    loadElementsFromPath(zonesPath, zonePixmaps, zoneButtons, zonesScrollArea, &MapEditor::zoneSelected);
}

void MapEditor::loadAvailableWeapons() {
    QString weaponsPath = getResourcesPath() + "weapons/";
    loadElementsFromPath(weaponsPath, weaponPixmaps, weaponButtons, weaponsScrollArea, &MapEditor::weaponSelected);
}

// Método para manejar la selección de tiles
void MapEditor::tileSelected(int id) {
    // Desactivar otros tipos de elementos
    currentSolidId = -1;
    currentZoneId = -1;
    currentWeaponId = -1;
    
    // Activar el tile seleccionado
    currentTileId = id;
    qDebug() << "Tile seleccionado:" << id;
    
    // Actualizar visualización de botones seleccionados
    updateSelectedButtonStyle(tileButtons, id);
    updateSelectedButtonStyle(solidButtons, -1); // Deseleccionar
    updateSelectedButtonStyle(zoneButtons, -1); // Deseleccionar
    updateSelectedButtonStyle(weaponButtons, -1); // Deseleccionar
}

// Método para manejar la selección de sólidos
void MapEditor::solidSelected(int id) {
    // Desactivar otros tipos de elementos
    currentTileId = -1;
    currentZoneId = -1;
    currentWeaponId = -1;
    
    // Activar el sólido seleccionado
    currentSolidId = id;
    qDebug() << "Sólido seleccionado:" << id;
    
    // Actualizar visualización de botones seleccionados
    updateSelectedButtonStyle(solidButtons, id);
    updateSelectedButtonStyle(tileButtons, -1); // Deseleccionar
    updateSelectedButtonStyle(zoneButtons, -1); // Deseleccionar
    updateSelectedButtonStyle(weaponButtons, -1); // Deseleccionar
}

// Método para manejar la selección de zonas
void MapEditor::zoneSelected(int id) {
    // Desactivar otros tipos de elementos
    currentTileId = -1;
    currentSolidId = -1;
    currentWeaponId = -1;
    
    // Activar la zona seleccionada
    currentZoneId = id;
    qDebug() << "Zona seleccionada:" << id;
    
    // Actualizar visualización de botones seleccionados
    updateSelectedButtonStyle(zoneButtons, id);
    updateSelectedButtonStyle(tileButtons, -1); // Deseleccionar
    updateSelectedButtonStyle(solidButtons, -1); // Deseleccionar
    updateSelectedButtonStyle(weaponButtons, -1); // Deseleccionar
}

// Método para manejar la selección de armas
void MapEditor::weaponSelected(int id) {
    // Desactivar otros tipos de elementos
    currentTileId = -1;
    currentSolidId = -1;
    currentZoneId = -1;
    
    // Activar el arma seleccionada
    currentWeaponId = id;
    qDebug() << "Arma seleccionada:" << id;
    
    // Actualizar visualización de botones seleccionados
    updateSelectedButtonStyle(weaponButtons, id);
    updateSelectedButtonStyle(tileButtons, -1); // Deseleccionar
    updateSelectedButtonStyle(solidButtons, -1); // Deseleccionar
    updateSelectedButtonStyle(zoneButtons, -1); // Deseleccionar
}

// Método para colocar un tile en la posición del clic
void MapEditor::placeTile(QPointF scenePos)
{
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
    
    // Buscar y eliminar CUALQUIER elemento existente en la misma posición
    QList<QGraphicsItem*> itemsToRemove;
    for (QGraphicsItem* item : scene->items()) {
        // Verificar si es un elemento de mapa (usando data)
        if ((item->data(1).isValid())) {
            QPoint itemGridPos = getTileGridPosition(item->pos());
            if (itemGridPos == gridPos) {
                itemsToRemove.append(item);
            }
        } 
    }
    
    // Remover de la escena y de la lista de elementos del mapa
    for (QGraphicsItem* itemToRemove : itemsToRemove) {
        // Remover el tile del registro placedTiles
        QPoint removeGridPos = getTileGridPosition(itemToRemove->pos());
        QPair<int, int> gridKey(removeGridPos.x(), removeGridPos.y());
        placedTiles.remove(gridKey);
        
        scene->removeItem(itemToRemove);
        delete itemToRemove;
    }
    
    // Crear un nuevo elemento tile sin comportamiento de drag and drop
    QPixmap tilePixmap = tilePixmaps[currentTileId];
    QGraphicsPixmapItem* tileItem = new QGraphicsPixmapItem(tilePixmap);
    tileItem->setPos(x, y);
    tileItem->setZValue(-0.5); // Por encima del fondo pero debajo de otros elementos
    
    // Almacenar metadatos en el item
    tileItem->setData(0, currentTileId); // ID del tile
    tileItem->setData(1, TILE); // Tipo de elemento (tile)
    
    // Añadir el nuevo tile a la escena
    scene->addItem(tileItem);
    
    // Guardar la referencia del tile colocado
    QPair<int, int> newGridKey(gridX, gridY);
    placedTiles[newGridKey] = currentTileId;
}

// Método para colocar un sólido en la posición del clic
void MapEditor::placeSolid(QPointF scenePos)
{
    if (currentSolidId < 0) {
        return; // No hay sólido seleccionado
    }
    
    QPoint gridPos = getTileGridPosition(scenePos);
    qreal x = gridPos.x() * 32.0;
    qreal y = gridPos.y() * 32.0;
    
    // Buscar y eliminar CUALQUIER elemento existente en la misma posición
    QList<QGraphicsItem*> itemsToRemove;
    for (QGraphicsItem* item : scene->items()) {
        // Verificar si es un elemento de mapa (usando data)
        if ((item->data(1).isValid())) {
            QPoint itemGridPos = getTileGridPosition(item->pos());
            if (itemGridPos == gridPos) {
                itemsToRemove.append(item);
            }
        }
    }
    
    // Remover de la escena y de la lista de elementos del mapa
    for (QGraphicsItem* itemToRemove : itemsToRemove) {
        // También eliminar el elemento de mapElements
        QPoint removeGridPos = getTileGridPosition(itemToRemove->pos());
        for (int i = 0; i < mapElements.size(); ++i) {
            MapElement* element = mapElements[i];
            QPointF elementPos = element->getPosition();
            if (elementPos.x() == removeGridPos.x() && 
                elementPos.y() == removeGridPos.y() && 
                element->getType() == SOLID_STRUCTURE) {
                mapElements.removeAt(i);
                delete element;
                break;
            }
        }
        scene->removeItem(itemToRemove);
        delete itemToRemove;
    }
    
    // Crear un nuevo elemento sólido
    QPixmap solidPixmap = solidPixmaps[currentSolidId];
    DragAndDrop* solidItem = new DragAndDrop(solidPixmap);
    solidItem->setPos(x, y);
    scene->addItem(solidItem);
    
    // Crear un nuevo elemento de mapa de tipo sólido
    QPointF worldPos(gridPos.x(), gridPos.y());
    MapElement* newElement = new MapElement(worldPos, SOLID_STRUCTURE);
    
    // Asociar el elemento gráfico con una identificación interna
    solidItem->setData(0, currentSolidId); // Almacenar el ID del sólido
    solidItem->setData(1, SOLID_STRUCTURE); // Almacenar el tipo de elemento
    
    // Añadir a la lista de elementos
    mapElements.append(newElement);
}

// Método para colocar una zona en la posición del clic
// Solo debe existir una zona de cada tipo en todo el mapa
void MapEditor::placeZone(QPointF scenePos)
{
    if (currentZoneId < 0) {
        return; // No hay zona seleccionada
    }
    
    QPoint gridPos = getTileGridPosition(scenePos);
    qreal x = gridPos.x() * 32.0;
    qreal y = gridPos.y() * 32.0;
    
    // PRIMERO: Eliminar cualquier zona DEL MISMO TIPO (currentZoneId) que ya exista en cualquier parte del mapa
    // Ya que solo debe haber una zona de cada tipo en todo el mapa
    QList<QGraphicsItem*> sameZoneTypeItems;
    
    for (QGraphicsItem* item : scene->items()) {
        DragAndDrop* existingItem = dynamic_cast<DragAndDrop*>(item);
        if (existingItem && 
            existingItem->data(1).toInt() == BOMB_ZONE && 
            existingItem->data(0).toInt() == currentZoneId) { // Verificar si es el mismo tipo de zona
            // Misma zona (tipo) encontrada, agregar para eliminar
            sameZoneTypeItems.append(existingItem);
            qDebug() << "Eliminando zona existente del tipo" << currentZoneId;
        }
    }
    
    // Eliminar todas las zonas del mismo tipo encontradas
    for (QGraphicsItem* itemToRemove : sameZoneTypeItems) {
        // Eliminar también de la lista de elementos internos
        QPoint removeGridPos = getTileGridPosition(itemToRemove->pos());
        for (int i = 0; i < mapElements.size(); ++i) {
            MapElement* element = mapElements[i];
            QPointF elementPos = element->getPosition();
            if (elementPos.x() == removeGridPos.x() && 
                elementPos.y() == removeGridPos.y() && 
                element->getType() == BOMB_ZONE) {
                mapElements.removeAt(i);
                delete element;
                break;
            }
        }
        scene->removeItem(itemToRemove);
        delete itemToRemove;
    }
    
    // SEGUNDO: Eliminar cualquier elemento que exista en la posición seleccionada
    QList<QGraphicsItem*> itemsAtPosition;
    for (QGraphicsItem* item : scene->items()) {
        // Verificar si es un elemento de mapa (usando data)
        if ((item->data(1).isValid())) {
            QPoint itemGridPos = getTileGridPosition(item->pos());
            if (itemGridPos == gridPos) {
                itemsAtPosition.append(item);
            }
        }
    }
    
    // Eliminar elementos en la posición seleccionada
    for (QGraphicsItem* itemToRemove : itemsAtPosition) {
        QPoint removeGridPos = getTileGridPosition(itemToRemove->pos());
        for (int i = 0; i < mapElements.size(); ++i) {
            MapElement* element = mapElements[i];
            QPointF elementPos = element->getPosition();
            if (elementPos.x() == removeGridPos.x() && 
                elementPos.y() == removeGridPos.y()) {
                mapElements.removeAt(i);
                delete element;
                break;
            }
        }
        scene->removeItem(itemToRemove);
        delete itemToRemove;
    }
    
    // Crear un nuevo elemento zona
    QPixmap zonePixmap = zonePixmaps[currentZoneId];
    DragAndDrop* zoneItem = new DragAndDrop(zonePixmap);
    zoneItem->setPos(x, y);
    scene->addItem(zoneItem);
    
    // Crear un nuevo elemento de mapa de tipo zona
    QPointF worldPos(gridPos.x(), gridPos.y());
    MapElement* newElement = new MapElement(worldPos, BOMB_ZONE);
    
    // Asociar el elemento gráfico con una identificación interna
    zoneItem->setData(0, currentZoneId); // Almacenar el ID de la zona
    zoneItem->setData(1, BOMB_ZONE); // Almacenar el tipo de elemento
    
    // Añadir a la lista de elementos
    mapElements.append(newElement);
}

// Método para colocar un arma en la posición del clic
void MapEditor::placeWeapon(QPointF scenePos)
{
    if (currentWeaponId < 0) {
        return; // No hay arma seleccionada
    }
    
    QPoint gridPos = getTileGridPosition(scenePos);
    qreal x = gridPos.x() * 32.0;
    qreal y = gridPos.y() * 32.0;
    
    // Buscar y eliminar CUALQUIER elemento existente en la misma posición
    QList<QGraphicsItem*> itemsToRemove;
    for (QGraphicsItem* item : scene->items()) {
        // Verificar si es un elemento de mapa (usando data)
        if ((item->data(1).isValid())) {
            QPoint itemGridPos = getTileGridPosition(item->pos());
            if (itemGridPos == gridPos) {
                itemsToRemove.append(item);
            }
        }
    }
    
    // Remover de la escena y de la lista de elementos del mapa
    for (QGraphicsItem* itemToRemove : itemsToRemove) {
        // También eliminar el elemento de mapElements
        QPoint removeGridPos = getTileGridPosition(itemToRemove->pos());
        for (int i = 0; i < mapElements.size(); ++i) {
            MapElement* element = mapElements[i];
            QPointF elementPos = element->getPosition();
            if (elementPos.x() == removeGridPos.x() && 
                elementPos.y() == removeGridPos.y() && 
                element->getType() == WEAPON) {
                mapElements.removeAt(i);
                delete element;
                break;
            }
        }
        scene->removeItem(itemToRemove);
        delete itemToRemove;
    }
    
    // Crear un nuevo elemento arma
    QPixmap weaponPixmap = weaponPixmaps[currentWeaponId];
    DragAndDrop* weaponItem = new DragAndDrop(weaponPixmap);
    weaponItem->setPos(x, y);
    scene->addItem(weaponItem);
    
    // Crear un nuevo elemento de mapa de tipo arma
    QPointF worldPos(gridPos.x(), gridPos.y());
    MapElement* newElement = new MapElement(worldPos, WEAPON);
    
    // Asociar el elemento gráfico con una identificación interna
    weaponItem->setData(0, currentWeaponId); // Almacenar el ID del arma
    weaponItem->setData(1, WEAPON); // Almacenar el tipo de elemento
    
    // Añadir a la lista de elementos
    mapElements.append(newElement);
}

// Método para eliminar un tile en la posición del clic derecho
void MapEditor::removeTile(QPointF scenePos) {
    // Convertir la posición de la escena a coordenadas de cuadrícula
    QPoint gridPos = getTileGridPosition(scenePos);
    QPair<int, int> gridKey(gridPos.x(), gridPos.y());
    
    // Buscar y eliminar cualquier tile en la posición
    bool tileRemoved = false;
    QList<QGraphicsItem*> itemsToRemove;
    
    // Buscar CUALQUIER elemento del mapa en esa posición
    for (QGraphicsItem* item : scene->items()) {
        // Verificamos si tiene metadatos de elementos del mapa
        if (item->data(1).isValid()) {
            QPoint itemGridPos = getTileGridPosition(item->pos());
            if (itemGridPos == gridPos) {
                // Si es un tile, marcamos que hemos eliminado un tile para limpiar el registro
                if (item->data(1).toInt() == TILE) {
                    tileRemoved = true;
                }
                itemsToRemove.append(item);
            }
        }
    }
    
    // Eliminar los tiles encontrados
    for (QGraphicsItem* item : itemsToRemove) {
        scene->removeItem(item);
        delete item;
    }
    
    // Eliminar del registro de tiles colocados si se eliminó alguno
    if (tileRemoved) {
        placedTiles.remove(gridKey);
    }
}

// Método para eliminar cualquier elemento en la posición del clic derecho
void MapEditor::removeElementAt(QPointF scenePos) {
    // Convertir la posición de la escena a coordenadas de cuadrícula
    QPoint gridPos = getTileGridPosition(scenePos);
    
    // Buscar algún elemento en esta posición
    QPointF worldPos(gridPos.x(), gridPos.y());
    
    // Buscar elementos en la escena en esta posición
    QList<QGraphicsItem*> itemsAtPos = scene->items(worldPos.x() * 32 + 16, worldPos.y() * 32 + 16, 1, 1, Qt::IntersectsItemShape, Qt::DescendingOrder);
    
    for (QGraphicsItem* item : itemsAtPos) {
        // Verificar si es un DragAndDrop
        DragAndDrop* dragItem = dynamic_cast<DragAndDrop*>(item);
        if (dragItem) {
            // Solo eliminamos elementos que no son tiles (los tiles se manejan en removeTile)
            if (dragItem->data(1).isValid()) {
                int elementType = dragItem->data(1).toInt();
                if (elementType == SOLID_STRUCTURE || elementType == BOMB_ZONE || elementType == WEAPON) {
                    // Encontrar y eliminar el MapElement correspondiente
                    for (int i = 0; i < mapElements.size(); i++) {
                        MapElement* element = mapElements.at(i);
                        if (element->getPosition() == worldPos && element->getType() == elementType) {
                            // Eliminar el elemento del mapa
                            scene->removeItem(dragItem);
                            delete dragItem;
                            
                            // Eliminar el elemento lógico
                            mapElements.removeAt(i);
                            delete element;
                            return; // Solo eliminamos un elemento a la vez
                        }
                    }
                }
            }
        }
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
    try {
        // Se podría usar para cambiar colores, estilos o comportamientos específicos
        // según el tipo de terreno seleccionado
        switch (terrainType) {
            case 0: // DESERT
                setWindowTitle("Counter-Strike Editor - Mapa Desierto");
                break;
            case 1: // AZTEC_VILLAGE
                setWindowTitle("Counter-Strike Editor - Mapa Pueblito Azteca");
                break;
            case 2: // TRAINING_ZONE
                setWindowTitle("Counter-Strike Editor - Mapa Zona de Entrenamiento");
                break;
            default:
                setWindowTitle("Counter-Strike Editor de Mapas");
                break;
        }
    } catch (...) {
        qCritical() << "Excepción en updateTerrainUI";
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
        if (dynamic_cast<DragAndDrop*>(item)) {
            itemsToRemove.append(item);
        }
        // También eliminar tiles (son QGraphicsPixmapItem con zValue -0.5)
        else if (QGraphicsItem* pixmapItem = dynamic_cast<QGraphicsItem*>(item)) {
            if (pixmapItem->zValue() == -0.5 && dynamic_cast<TileItem*>(pixmapItem)) {
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
                    TileItem* tileItem = new TileItem(tilePixmaps[tile->getTileId()], tile->getTileId(), QString("Tile_%1").arg(tile->getTileId()), scene);
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
        if (DragAndDrop* dragItem = dynamic_cast<DragAndDrop*>(item)) {
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

// Método para actualizar el indicador de selección cuando el cursor se mueve
void MapEditor::updateSelectionIndicator(QPointF scenePos)
{
    // Obtener la posición de cuadrícula
    QPoint gridPos = getTileGridPosition(scenePos);
    
    // Verificar que esté dentro de los límites de la cuadrícula
    if (gridPos.x() >= 0 && gridPos.x() < 1000/32 && gridPos.y() >= 0 && gridPos.y() < 1000/32) {
        // Calcular la posición exacta en la escena (alineada con la cuadrícula)
        qreal x = gridPos.x() * 32.0;
        qreal y = gridPos.y() * 32.0;
        
        // Actualizar la posición del indicador
        if (selectionIndicator) {
            selectionIndicator->setPos(x, y);
            selectionIndicator->show(); // Asegurar que sea visible
        }
    }
}

bool MapEditor::eventFilter(QObject* watched, QEvent* event)
{
    try {
        // Verificación de validez de punteros para prevenir segmentation fault
        if (!watched || !event || !view || !view->viewport()) {
            qWarning() << "eventFilter: Puntero nulo detectado";
            return false;
        }
        
        // Capturar eventos de ratón en la vista
        if (watched == view->viewport()) {
            // Asegurarse de que el evento es realmente un evento de ratón
            if (event->type() != QEvent::MouseButtonPress && 
                event->type() != QEvent::MouseButtonRelease && 
                event->type() != QEvent::MouseMove) {
                return QMainWindow::eventFilter(watched, event);
            }
            
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QPointF scenePos = view->mapToScene(mouseEvent->pos());
            
            // Actualizar el indicador de selección para cualquier evento de mouse
            if (event->type() == QEvent::MouseMove) {
                updateSelectionIndicator(scenePos);
            }
            
            if (event->type() == QEvent::MouseButtonPress) {
                if (mouseEvent->button() == Qt::LeftButton) {
                    // Verificar que la escena existe antes de proceder
                    if (!scene) {
                        qWarning() << "eventFilter: Intentando acceder a una escena nula";
                        return false;
                    }
                    
                    // Colocar elemento según lo que esté seleccionado actualmente
                    if (currentTileId >= 0 && tilePixmaps.contains(currentTileId)) {
                        // Colocar un tile si está disponible en el mapa
                        placeTile(scenePos);
                        return true;
                    } else if (currentSolidId >= 0 && solidPixmaps.contains(currentSolidId)) {
                        // Colocar un sólido
                        placeSolid(scenePos);
                        return true;
                    } else if (currentZoneId >= 0 && zonePixmaps.contains(currentZoneId)) {
                        // Colocar una zona
                        placeZone(scenePos);
                        return true;
                    } else if (currentWeaponId >= 0 && weaponPixmaps.contains(currentWeaponId)) {
                        // Colocar un arma
                        placeWeapon(scenePos);
                        return true;
                    }
                } else if (mouseEvent->button() == Qt::RightButton) {
                    // Clic derecho para eliminar elementos
                    if (currentTileId >= 0) {
                        removeTile(scenePos);
                        return true;
                    } else if (currentSolidId >= 0 || currentZoneId >= 0 || currentWeaponId >= 0) {
                        // Para otros elementos, intentar eliminar cualquier elemento en esa posición
                        removeElementAt(scenePos);
                        return true;
                    }
                }
            }
        }
        
        // Dejar que Qt maneje otros eventos
        return QMainWindow::eventFilter(watched, event);
    } catch (const std::exception& e) {
        qCritical() << "Excepción en eventFilter:" << e.what();
        return false;
    } catch (...) {
        qCritical() << "Excepción desconocida en eventFilter";
        return false;
    }
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
        if (DragAndDrop* dragItem = dynamic_cast<DragAndDrop*>(item)) {
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
