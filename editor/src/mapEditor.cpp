#include "mapEditor.h"

#include <QCoreApplication>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QImage>
#include <QPainter>
#include <QScrollBar>
#include <iostream>

#include "backgroundItem.h"
#include "dragAndDrop.h"
#include "tileItem.h"

MapEditor::MapEditor(QWidget* parent):
        QMainWindow(parent), currentBackground(nullptr), currentTerrainType(0) {
    // Configurar una escena de 1000x1000 para el mapa de tiles
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1000, 1000);  // Tamaño exacto para mapa de 1000x1000

    // Inicializar los button groups
    tileButtons = new QButtonGroup(this);

    view = new QGraphicsView(scene, this);

    view->setRenderHint(QPainter::Antialiasing, true);
    view->setRenderHint(QPainter::SmoothPixmapTransform, true);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    // Configurar scrollbars
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);  // Habilitamos scrolling
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);    // Habilitamos scrolling

    view->setSceneRect(scene->sceneRect());
    view->setFixedSize(800, 600);  // Tamaño de la ventana de visualización

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
    QGroupBox* fileGroup = new QGroupBox("Archivo", toolPanel);
    QVBoxLayout* fileLayout = new QVBoxLayout(fileGroup);

    QPushButton* saveMapButton = new QPushButton("Guardar Mapa", fileGroup);
    QPushButton* preloadMapButton = new QPushButton("Precargar", fileGroup);
    preloadMapButton->setToolTip("Llenar todo el mapa con un tile base del terreno seleccionado");

    fileLayout->addWidget(saveMapButton);
    fileLayout->addWidget(preloadMapButton);
    fileGroup->setLayout(fileLayout);

    // Sección de control de zoom
    QGroupBox* zoomGroup = new QGroupBox("Control de Zoom", toolPanel);
    QHBoxLayout* zoomLayout = new QHBoxLayout(zoomGroup);

    QPushButton* zoomInButton = new QPushButton("+", zoomGroup);
    QPushButton* zoomOutButton = new QPushButton("-", zoomGroup);
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
    if (toolPanelLayout) {  // Si existe, añadimos los widgets restantes
        toolPanelLayout->addWidget(zoomGroup);
        toolPanelLayout->addWidget(fileGroup);
        toolPanelLayout->addStretch(1);  // Espacio flexible al final
    }

    // Conectar señales
    connect(terrainCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MapEditor::backgroundSelection);

    // Conectar botones de carga y guardado
    connect(saveMapButton, &QPushButton::clicked, this, &MapEditor::generarMapaClicked);
    connect(preloadMapButton, &QPushButton::clicked, this, &MapEditor::preloadMapWithBaseTile);

    // Conectar botones de zoom
    connect(zoomInButton, &QPushButton::clicked, this, &MapEditor::zoomIn);
    connect(zoomOutButton, &QPushButton::clicked, this, &MapEditor::zoomOut);

    // Instalamos un event filter en la vista para capturar clicks y movimientos
    view->viewport()->installEventFilter(this);

    // No necesitamos un indicador en la cuadrícula porque resaltaremos los elementos en el menú

    // Estos métodos se llamarán más abajo en backgroundSelection(0)

    // Crear el layout principal con el panel de herramientas y la vista
    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addWidget(toolPanel);
    mainLayout->addWidget(view, 1);  // La vista toma el espacio disponible

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Configurar tamaño de la ventana principal
    resize(1024, 700);
    setWindowTitle("Counter-Strike Editor de Mapas");

    try {
        // Inicializar con el fondo de desierto por defecto - usar try-catch para identificar donde
        // está el segfault
        qDebug() << "A punto de llamar a backgroundSelection(0)";
        backgroundSelection(0);  // Carga los tiles disponibles y configura el fondo
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
void MapEditor::updateSelectedButtonStyle(QButtonGroup* buttonGroup, int selectedId) {
    // Estilo para resaltar el botón seleccionado
    const QString selectedStyle =
            "QPushButton { border: 2px solid #FFD700; background-color: rgba(255, 215, 0, 0.3); }";
    const QString normalStyle = "";

    // Recorrer todos los botones y actualizar su estilo
    for (QAbstractButton* button: buttonGroup->buttons()) {
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

MapEditor::~MapEditor() {
    // Desconectar todas las señales para evitar que se activen durante la destrucción
    disconnect(this);  // Desconecta todas las conexiones que involucran a esta instancia

    // Primero intentar remover elementos de la escena para evitar accesos inválidos
    if (scene) {
        scene->clear();  // Limpia todos los items de la escena
    }

    // Limpiar primero currentBackground para evitar accesos inválidos
    if (currentBackground) {
        currentBackground = nullptr;  // Evita acceso a memoria liberada
    }

    // Limpiar los mapas de tiles
    tilePixmaps.clear();
    placedTiles.clear();
}

// Método eliminado - ahora los elementos se colocan directamente con el clic

// Método eliminado - ahora los elementos se colocan directamente con el clic

// Método eliminado - ahora los elementos se colocan directamente con el clic
// Método eliminado - ahora los elementos se colocan directamente con el clic

void MapEditor::backgroundSelection(int index) {
    // Proteger contra llamadas recursivas
    static bool isUpdating = false;
    if (isUpdating)
        return;
    isUpdating = true;

    // Borrar el fondo anterior si existe
    if (currentBackground) {
        scene->removeItem(currentBackground);
        delete currentBackground;
        currentBackground = nullptr;
    }

    // Eliminar todos los tiles que pudieran existir en la escena
    QList<QGraphicsItem*> tilesToRemove;
    for (QGraphicsItem* item: scene->items()) {
        QGraphicsPixmapItem* tileItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        if (tileItem && tileItem->data(1).toInt() == TILE) {
            tilesToRemove.append(tileItem);
        }
    }

    // Eliminar los tiles encontrados
    for (QGraphicsItem* item: tilesToRemove) {
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
    currentBackground->setZValue(-1000);  // Asegurarse de que el fondo esté detrás de todo

    // También eliminar todos los elementos del mapa ya que estamos cambiando el entorno
    for (QGraphicsItem* item: scene->items()) {
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
    // Pero evitar la recursion infinita que ocurre porque loadAvailableTiles llama a este método
    // indirectamente
    loadAvailableTiles();

    // Actualizar la interfaz según el terreno
    updateTerrainUI(index);

    isUpdating = false;
}

// Método para aumentar el zoom
void MapEditor::zoomIn() {
    // Aumentar el zoom en 25%
    currentZoomFactor += 0.25;
    // Limitar el zoom máximo
    if (currentZoomFactor > 3.0) {
        currentZoomFactor = 3.0;
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
    QPointF centerPoint =
            view->mapToScene(view->viewport()->width() / 2, view->viewport()->height() / 2);

    // Aplicar la transformación
    view->resetTransform();
    view->scale(currentZoomFactor, currentZoomFactor);

    // Centrar en el mismo punto después del zoom
    view->centerOn(centerPoint);

    qDebug() << "Aplicado factor de zoom: " << currentZoomFactor;
}

// Obtener un color representativo para cada tipo de terreno
QColor MapEditor::getTerrainColor(int terrainType) {
    switch (terrainType) {
        case DESERT:
            return QColor(194, 178, 128);  // Color arena
        case AZTEC_VILLAGE:
            return QColor(120, 145, 93);  // Verde azteca
        case TRAINING_ZONE:
            return QColor(150, 150, 150);  // Gris entrenamiento
        default:
            return QColor(194, 178, 128);  // Arena por defecto
    }
}

// Implementación de funciones de escala
float MapEditor::pixelToWorldX(float pixelX, float elementWidth) {
    // elementWidth no se usa actualmente pero podría usarse para ajustes de posición
    (void)elementWidth;  // Evitar advertencia de parámetro no utilizado
    return pixelX / 25.6f;
}

float MapEditor::pixelToWorldY(float pixelY, float elementHeight) {
    // elementHeight no se usa actualmente pero podría usarse para ajustes de posición
    (void)elementHeight;  // Evitar advertencia de parámetro no utilizado
    return pixelY / 25.6f;
}

float MapEditor::worldToPixelX(float worldX) { return worldX * 25.6f; }

float MapEditor::worldToPixelY(float worldY) { return worldY * 25.6f; }

// Método para obtener la ruta base a los recursos
QString MapEditor::getResourcesPath() {
    QString projectDir = QDir::currentPath();

    // Imprimimos el directorio actual para debug
    qDebug() << "Directorio actual:" << projectDir;

    // Opciones de rutas relativas desde diferentes puntos de compilación
    QStringList possiblePaths = {"../editor/resources/",
                                 "resources/",
                                 "../resources/",
                                 "../../editor/resources/",
                                 QCoreApplication::applicationDirPath() + "/resources/",
                                 QCoreApplication::applicationDirPath() + "/../editor/resources/"};

    // Buscar la primera ruta válida
    for (const QString& path: possiblePaths) {
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

// // Método para obtener la ruta completa de un recurso según su tipo
// QString MapEditor::getResourcePath(int elementType, int subType) {
//     QString basePath = getResourcesPath();

//     switch (elementType) {
//         case TEAM_SPAWN_CT:
//             return basePath + "player/ct.png";

//         case TEAM_SPAWN_T:
//             return basePath + "player/t.png";

//         case BOMB_ZONE:
//             return basePath + "objects/bomb_zone.png";

//         case SOLID_STRUCTURE:
//             // Podría tener diferentes tipos de estructuras según subType
//             return basePath + "objects/box.png";

//         case WEAPON:
//             // Diferentes tipos de armas según el subType
//             switch (subType) {
//                 case 0: // Pistola
//                     return basePath + "weapons/pistol.png";
//                 case 1: // Rifle
//                     return basePath + "weapons/rifle.png";
//                 case 2: // Sniper
//                     return basePath + "weapons/sniper.png";
//                 case 3: // Shotgun
//                     return basePath + "weapons/shotgun.png";
//                 default:
//                     return basePath + "weapons/pistol.png";
//             }

//         default:
//             qWarning() << "Tipo de elemento desconocido:" << elementType;
//             return basePath + "unknown.png";
//     }
// }

// Nuevo método para cortar una imagen de tileset en tiles individuales
QMap<int, QPixmap> MapEditor::sliceTilesetImage(const QString& tilesetPath, int tileWidth,
                                                int tileHeight) {
    QMap<int, QPixmap> tileMap;

    // Cargar la imagen del tileset
    QPixmap tilesetImage(tilesetPath);
    if (tilesetImage.isNull()) {
        qWarning() << "No se pudo cargar el tileset:" << tilesetPath;
        return tileMap;
    }

    // Dimensiones del tileset
    int tilesetWidth = tilesetImage.width();
    int tilesetHeight = tilesetImage.height();

    // Calcular número de filas y columnas
    int cols = tilesetWidth / tileWidth;
    int rows = tilesetHeight / tileHeight;

    // ID inicial para los tiles
    int tileId = 1;  // Empezamos desde 1 como solicitó el usuario

    // Cortar el tileset en tiles individuales
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            // Calcular la posición del tile en la imagen original
            int posX = x * tileWidth;
            int posY = y * tileHeight;

            // Cortar el tile
            QPixmap tilePix = tilesetImage.copy(posX, posY, tileWidth, tileHeight);

            if (!tilePix.isNull()) {
                // Almacenar el tile con su ID
                tileMap[tileId] = tilePix;
                tileId++;
            }
        }
    }

    // Verificar que existan al menos 3 tiles para los especiales
    if (tileMap.size() >= 3) {
        qDebug() << "Tiles especiales identificados:";
        qDebug() << "Tile ID 1: Spawn Anti-Terroristas (CT)";
        qDebug() << "Tile ID 2: Spawn Terroristas (T)";
        qDebug() << "Tile ID 3: Bombsite";
    } else {
        qWarning() << "¡ADVERTENCIA! El tileset no tiene suficientes tiles (mínimo 3). Los "
                      "primeros 3 tiles son requeridos para: CT spawn, T spawn y Bombsite";
    }

    qDebug() << "Se cortaron" << tileMap.size() << "tiles del tileset" << tilesetPath;
    return tileMap;
}

void MapEditor::loadAvailableTiles() {
    try {
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

        // Limpiar los pixmaps y botones anteriores
        tilePixmaps.clear();
        QList<QAbstractButton*> buttons = tileButtons->buttons();
        for (QAbstractButton* button: buttons) {
            tileButtons->removeButton(button);
            delete button;
        }

        // Desconectar conexiones anteriores para evitar duplicados
        disconnect(tileButtons, nullptr, this, nullptr);

        // Reconectar la señal de selección
        connect(tileButtons, &QButtonGroup::idClicked, this, &MapEditor::tileSelected);

        // Cargar los tilesets según el tipo de terreno actual
        QString terrainType;
        QString tilesetFile;

        switch (currentTerrainType) {
            case 0:  // DESERT_TERRAIN
                terrainType = "Desert";
                tilesetFile = "desert_tiles.png";
                break;
            case 1:  // AZTEC_VILLAGE
                terrainType = "Aztec";
                tilesetFile = "aztec_tiles.png";
                break;
            case 2:  // TRAINING_GROUND
                terrainType = "Training";
                tilesetFile = "training_tiles.png";
                break;
            default:
                terrainType = "Desert";
                tilesetFile = "desert_tiles.png";
                break;
        }

        // Ruta al tileset
        QString tilesetPath = QString("../client/assets/tiles/%1").arg(tilesetFile);

        // Cortar el tileset en tiles individuales (32x32 pixeles)
        tilePixmaps = sliceTilesetImage(tilesetPath, 32, 32);

        // Obtener el widget contenedor para los botones
        QWidget* elementsContainer = tilesScrollArea->widget();
        if (!elementsContainer) {
            elementsContainer = new QWidget();
            tilesScrollArea->setWidget(elementsContainer);
            tilesScrollArea->setWidgetResizable(true);
        }

        // Obtener o crear el layout
        QGridLayout* elementsGridLayout = qobject_cast<QGridLayout*>(elementsContainer->layout());
        if (!elementsGridLayout) {
            if (elementsContainer->layout()) {
                delete elementsContainer->layout();
            }
            elementsGridLayout = new QGridLayout(elementsContainer);
            elementsGridLayout->setSpacing(2);
            elementsGridLayout->setContentsMargins(2, 2, 2, 2);
        }

        // Crear botones para cada tile
        int row = 0;
        int col = 0;

        for (auto it = tilePixmaps.begin(); it != tilePixmaps.end(); ++it) {
            int tileId = it.key();
            QPixmap tilePixmap = it.value();

            // Crear un botón con una miniatura del tile
            QPushButton* tileButton = new QPushButton();
            tileButton->setFixedSize(40, 40);

            // Crear una versión escalada para el botón
            QPixmap scaledPixmap =
                    tilePixmap.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QIcon buttonIcon(scaledPixmap);
            tileButton->setIcon(buttonIcon);
            tileButton->setIconSize(QSize(36, 36));

            // Identificar y resaltar los tiles especiales
            if (tileId == 1) {
                tileButton->setToolTip("Anti-Terroristas (CT) - Tile 1");
                tileButton->setStyleSheet("QPushButton {border: 2px solid blue; background-color: "
                                          "rgba(0, 0, 255, 40);}");
            } else if (tileId == 2) {
                tileButton->setToolTip("Terroristas (T) - Tile 2");
                tileButton->setStyleSheet("QPushButton {border: 2px solid red; background-color: "
                                          "rgba(255, 0, 0, 40);}");
            } else if (tileId == 3) {
                tileButton->setToolTip("Bombsite - Tile 3");
                tileButton->setStyleSheet("QPushButton {border: 2px solid yellow; "
                                          "background-color: rgba(255, 255, 0, 40);}");
            } else {
                tileButton->setToolTip(QString("Tile %1").arg(tileId));
                tileButton->setStyleSheet("");  // Inicializar sin estilo especial
            }

            // Añadir al grupo de botones
            tileButtons->addButton(tileButton, tileId);

            // Añadir a la cuadrícula
            elementsGridLayout->addWidget(tileButton, row, col % 3);

            // Actualizar posición para el siguiente elemento
            col++;
            if (col % 3 == 0) {
                row++;
                col = 0;
            }
        }

        qDebug() << "Se cargaron" << tilePixmaps.size() << "tiles del terreno" << terrainType;

        // Verificar todos los punteros antes de cargar otros elementos
        if (!tilesScrollArea || !tileButtons || !weaponsScrollArea || !weaponButtons) {
            qWarning() << "Algunos punteros son nullptr! No se cargarán más recursos para evitar "
                          "fallos.";
            return;
        }

        // Solo cargar armas adicionales
        loadAvailableWeapons();
    } catch (const std::exception& e) {
        qCritical() << "Excepción capturada en loadAvailableTiles:" << e.what();
    } catch (...) {
        qCritical() << "Excepción desconocida capturada en loadAvailableTiles";
    }
}

// Se eliminaron los métodos loadAvailableSolids, loadAvailableZones, loadAvailableBombZones y
// loadAvailableExtraTiles como parte de la simplificación para trabajar solo con tiles y armas

void MapEditor::loadAvailableWeapons() {
    // Ruta a los assets de armas en client/assets/weapons/
    QString weaponsPath = "../client/assets/weapons/";

    // Comprobar que el scroll area y el grupo de botones existen
    if (!weaponsScrollArea || !weaponButtons) {
        qWarning() << "weaponsScrollArea o weaponButtons son nullptr! No se cargarán armas.";
        return;
    }

    // Limpiar los pixmaps y botones anteriores
    weaponPixmaps.clear();
    QList<QAbstractButton*> buttons = weaponButtons->buttons();
    for (QAbstractButton* button: buttons) {
        weaponButtons->removeButton(button);
        delete button;
    }

    // Desconectar conexiones anteriores para evitar duplicados
    disconnect(weaponButtons, nullptr, this, nullptr);

    // Reconectar la señal de selección
    connect(weaponButtons, &QButtonGroup::idClicked, this, &MapEditor::weaponSelected);

    // Verificar que existe el directorio
    QDir dir(weaponsPath);
    if (!dir.exists()) {
        qWarning() << "El directorio de armas no existe:" << weaponsPath;
        return;
    }

    // Filtrar por archivos de imagen
    QStringList filters;
    filters << "*.png"
            << "*.jpg"
            << "*.bmp"
            << "*.gif";
    dir.setNameFilters(filters);
    QStringList files = dir.entryList(filters, QDir::Files, QDir::Name);

    // Obtener el widget contenedor para los botones
    QWidget* elementsContainer = weaponsScrollArea->widget();
    if (!elementsContainer) {
        elementsContainer = new QWidget();
        weaponsScrollArea->setWidget(elementsContainer);
        weaponsScrollArea->setWidgetResizable(true);
    }

    // Obtener o crear el layout
    QGridLayout* elementsGridLayout = qobject_cast<QGridLayout*>(elementsContainer->layout());
    if (!elementsGridLayout) {
        if (elementsContainer->layout()) {
            delete elementsContainer->layout();
        }
        elementsGridLayout = new QGridLayout(elementsContainer);
        elementsGridLayout->setSpacing(2);
        elementsGridLayout->setContentsMargins(2, 2, 2, 2);
    }

    // ID inicial para armas
    int weaponId = 1;
    int row = 0;
    int col = 0;

    // Cargar cada archivo de imagen como un arma
    for (const QString& file: files) {
        QString fullPath = weaponsPath + file;
        QPixmap weaponPixmap(fullPath);

        if (weaponPixmap.isNull()) {
            qWarning() << "No se pudo cargar el arma:" << fullPath;
            continue;
        }

        // Guardar el pixmap con su ID
        weaponPixmaps[weaponId] = weaponPixmap;

        // Crear un botón con una miniatura del arma
        QPushButton* weaponButton = new QPushButton();
        weaponButton->setFixedSize(60, 60);

        // Crear una versión escalada para el botón
        // Escalar manteniendo la proporción
        QPixmap scaledPixmap =
                weaponPixmap.scaled(56, 56, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QIcon buttonIcon(scaledPixmap);
        weaponButton->setIcon(buttonIcon);
        weaponButton->setIconSize(QSize(56, 56));

        // Obtener el nombre del archivo sin la extensión como nombre del arma
        QString weaponName = QFileInfo(file).baseName();
        weaponButton->setToolTip(weaponName);

        // Añadir al grupo de botones
        weaponButtons->addButton(weaponButton, weaponId);

        // Añadir a la cuadrícula
        elementsGridLayout->addWidget(weaponButton, row, col % 2);

        // Actualizar posición para el siguiente elemento
        col++;
        if (col % 2 == 0) {
            row++;
            col = 0;
        }

        weaponId++;
    }

    qDebug() << "Se cargaron" << weaponPixmaps.size() << "armas";
}

// Método para manejar la selección de tiles
void MapEditor::tileSelected(int id) {
    // Desactivar el arma seleccionada
    currentWeaponId = -1;

    // Activar el tile seleccionado
    currentTileId = id;
    qDebug() << "Tile seleccionado:" << id;

    // Actualizar visualización de botones seleccionados
    updateSelectedButtonStyle(tileButtons, id);
    updateSelectedButtonStyle(weaponButtons, -1);  // Deseleccionar
}

// Se eliminaron los métodos solidSelected, zoneSelected, bombZoneSelected y extraTileSelected
// como parte de la simplificación para trabajar solo con tiles y armas

// Método para manejar la selección de armas
void MapEditor::weaponSelected(int id) {
    // Desactivar el tile seleccionado
    currentTileId = -1;

    // Activar el arma seleccionada
    currentWeaponId = id;
    qDebug() << "Arma seleccionada:" << id;

    // Actualizar visualización de botones seleccionados
    updateSelectedButtonStyle(weaponButtons, id);
    updateSelectedButtonStyle(tileButtons, -1);  // Deseleccionar
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

    QPair<int, int> gridKey(gridX, gridY);
    // Evitar colocar el mismo tile si ya está en esa celda con el mismo ID
    if (placedTiles.contains(gridKey) && placedTiles[gridKey] == currentTileId) {
        return;  // Ya está colocado, no hacer nada
    }

    // Evitar colocar fuera de los límites
    if (gridX < 0 || gridX >= 31 || gridY < 0 || gridY >= 31) {
        return;
    }

    // Para tiles especiales (1, 2 y 3), verificar si ya existe uno en el mapa
    if (currentTileId >= 1 && currentTileId <= 3) {
        // Buscar si ya existe un tile con este ID en el mapa
        QPair<int, int> existingPos(-1, -1);
        bool tileExists = false;

        QMap<QPair<int, int>, int>::const_iterator it;
        for (it = placedTiles.constBegin(); it != placedTiles.constEnd(); ++it) {
            if (it.value() == currentTileId) {
                existingPos = it.key();
                tileExists = true;
                break;
            }
        }

        if (tileExists) {
            // Eliminar el tile especial existente
            for (QGraphicsItem* item: scene->items()) {
                if (item->data(1).isValid() && item->data(1).toInt() == TILE &&
                    item->data(0).toInt() == currentTileId) {
                    QPoint itemGridPos = getTileGridPosition(item->pos());
                    if (QPair<int, int>(itemGridPos.x(), itemGridPos.y()) == existingPos) {
                        scene->removeItem(item);
                        delete item;
                        placedTiles.remove(existingPos);

                        QString tileType;
                        if (currentTileId == 1)
                            tileType = "Anti-Terrorista (CT)";
                        else if (currentTileId == 2)
                            tileType = "Terrorista (T)";
                        else if (currentTileId == 3)
                            tileType = "Zona Bomba";

                        qDebug() << "Se eliminó el tile especial anterior de tipo" << tileType
                                 << "en posición (" << existingPos.first << ","
                                 << existingPos.second << ")";
                        break;
                    }
                }
            }
        }
    }

    // Calcular la posición exacta en la escena
    qreal x = gridX * 32.0;
    qreal y = gridY * 32.0;

    // Para tiles especiales (ID 1, 2 o 3), eliminar cualquier otro tile del mismo tipo en TODO el
    // mapa Esto asegura que solo haya un tile para CT spawn (ID 1), uno para T spawn (ID 2) y un
    // bombsite (ID 3)
    if (currentTileId >= 1 && currentTileId <= 3) {
        // Buscar en todo el mapa tiles del mismo tipo
        QList<QGraphicsItem*> specialTilesToRemove;
        for (QGraphicsItem* item: scene->items()) {
            if (item->data(1).isValid() && item->data(1).toInt() == TILE &&
                item->data(0).isValid() && item->data(0).toInt() == currentTileId) {
                specialTilesToRemove.append(item);
                qDebug() << "Eliminando tile especial existente con ID:" << currentTileId
                         << "en posición:" << getTileGridPosition(item->pos());
            }
        }

        // Eliminar los tiles especiales encontrados
        for (QGraphicsItem* itemToRemove: specialTilesToRemove) {
            QPoint removeGridPos = getTileGridPosition(itemToRemove->pos());
            QPair<int, int> gridKey(removeGridPos.x(), removeGridPos.y());
            placedTiles.remove(gridKey);

            scene->removeItem(itemToRemove);
            delete itemToRemove;
        }
    }

    // Buscar y eliminar CUALQUIER elemento existente en la misma posición
    QList<QGraphicsItem*> itemsToRemove;
    for (QGraphicsItem* item: scene->items()) {
        // Verificar si es un elemento de mapa (usando data)
        if ((item->data(1).isValid())) {
            QPoint itemGridPos = getTileGridPosition(item->pos());
            if (itemGridPos == gridPos) {
                itemsToRemove.append(item);
            }
        }
    }

    // Remover de la escena y de la lista de elementos del mapa
    for (QGraphicsItem* itemToRemove: itemsToRemove) {
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
    tileItem->setZValue(-0.5);  // Por encima del fondo pero debajo de otros elementos

    // Almacenar metadatos en el item
    tileItem->setData(0, currentTileId);  // ID del tile
    tileItem->setData(1, TILE);           // Tipo de elemento (tile)

    // Añadir el nuevo tile a la escena
    scene->addItem(tileItem);

    // Guardar la referencia del tile colocado
    QPair<int, int> newGridKey(gridX, gridY);
    placedTiles[newGridKey] = currentTileId;
}

// Método para colocar un extra-tile en la posición del clic
// Los métodos placeExtraTile, placeSolid, placeBombZone y placeZone se eliminaron
// como parte de la refactorización del editor para manejar solo tiles y armas

// Método para colocar un arma en la posición del clic
void MapEditor::placeWeapon(QPointF scenePos) {
    if (currentWeaponId < 0) {
        return;  // No hay arma seleccionada
    }

    QPoint gridPos = getTileGridPosition(scenePos);
    qreal x = gridPos.x() * 32.0;
    qreal y = gridPos.y() * 32.0;

    // Verificar si hay un tile o extra-tile en esta posición
    bool tileFound = false;
    QPair<int, int> gridKey(gridPos.x(), gridPos.y());

    // Comprobar en el mapa de tiles colocados
    if (placedTiles.contains(gridKey)) {
        tileFound = true;
    } else {
        // Comprobar si hay un tile o extra-tile en la escena
        for (QGraphicsItem* item: scene->items()) {
            if (item->data(1).isValid() &&
                (item->data(1).toInt() == TILE || item->data(1).toInt() == EXTRA_TILE)) {
                QPoint itemGridPos = getTileGridPosition(item->pos());
                if (itemGridPos == gridPos) {
                    tileFound = true;
                    break;
                }
            }
        }
    }

    // Si no hay un tile o extra-tile, no permitir la colocación del arma
    if (!tileFound) {
        QMessageBox::warning(this, "Colocación no válida",
                             "Las armas solo pueden colocarse sobre tiles o extra-tiles.");
        return;
    }

    // Buscar y eliminar CUALQUIER arma existente en la misma posición
    QList<QGraphicsItem*> itemsToRemove;
    for (QGraphicsItem* item: scene->items()) {
        // Verificar si es un arma (usando data)
        if (item->data(1).isValid() && item->data(1).toInt() == WEAPON) {
            QPoint itemGridPos = getTileGridPosition(item->pos());
            if (itemGridPos == gridPos) {
                itemsToRemove.append(item);
            }
        }
    }

    // Remover de la escena y de la lista de elementos del mapa
    for (QGraphicsItem* itemToRemove: itemsToRemove) {
        // También eliminar el elemento de mapElements
        QPoint removeGridPos = getTileGridPosition(itemToRemove->pos());
        for (int i = 0; i < mapElements.size(); ++i) {
            MapElement* element = mapElements[i];
            QPointF elementPos = element->getPosition();
            if (elementPos.x() == removeGridPos.x() && elementPos.y() == removeGridPos.y() &&
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
    DragAndDrop* weaponItem = new DragAndDrop(weaponPixmap, 1.0, scene);
    weaponItem->setPos(x, y);
    scene->addItem(weaponItem);

    // Configurar el elemento como arma para habilitar el arrastre
    weaponItem->setWeaponType(currentWeaponId);

    // Crear un nuevo elemento de mapa de tipo arma
    QPointF worldPos(gridPos.x(), gridPos.y());
    MapElement* newElement = new MapElement(worldPos, WEAPON);

    // Asociar el elemento gráfico con una identificación interna
    weaponItem->setData(0, currentWeaponId);  // Almacenar el ID del arma
    weaponItem->setData(1, WEAPON);           // Almacenar el tipo de elemento

    // Configurar el cursor para indicar que es arrastrable
    weaponItem->setCursor(Qt::OpenHandCursor);

    // Añadir a la lista de elementos
    mapElements.append(newElement);

    qDebug() << "Arma colocada sobre tile en posición:" << gridPos.x() << "," << gridPos.y();
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
    for (QGraphicsItem* item: scene->items()) {
        // Verificamos si tiene metadatos de elementos del mapa
        if (item->data(1).isValid()) {
            QPoint itemGridPos = getTileGridPosition(item->pos());
            if (itemGridPos == gridPos) {
                // Si es un tile, marcamos que hemos eliminado un tile para limpiar el registro
                // Verificamos ambos posibles tipos de tiles (TILE o TILE_ELEMENT)
                int elementType = item->data(1).toInt();
                if (elementType == TILE || elementType == TILE_ELEMENT) {
                    tileRemoved = true;
                    qDebug() << "Eliminando tile en posición:" << itemGridPos
                             << "con tipo:" << elementType;
                }
                itemsToRemove.append(item);
            }
        }
    }

    // Eliminar los tiles encontrados
    for (QGraphicsItem* item: itemsToRemove) {
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
    QList<QGraphicsItem*> itemsAtPos =
            scene->items(worldPos.x() * 32 + 16, worldPos.y() * 32 + 16, 1, 1,
                         Qt::IntersectsItemShape, Qt::DescendingOrder);

    for (QGraphicsItem* item: itemsAtPos) {
        // Verificar si es un DragAndDrop
        DragAndDrop* dragItem = dynamic_cast<DragAndDrop*>(item);
        if (dragItem) {
            // Solo eliminamos elementos que no son tiles (los tiles se manejan en removeTile)
            if (dragItem->data(1).isValid()) {
                int elementType = dragItem->data(1).toInt();
                if (elementType == SOLID_STRUCTURE || elementType == BOMB_ZONE ||
                    elementType == WEAPON) {
                    // Encontrar y eliminar el MapElement correspondiente
                    for (int i = 0; i < mapElements.size(); i++) {
                        MapElement* element = mapElements.at(i);
                        if (element->getPosition() == worldPos &&
                            element->getType() == elementType) {
                            // Eliminar el elemento del mapa
                            scene->removeItem(dragItem);
                            delete dragItem;

                            // Eliminar el elemento lógico
                            mapElements.removeAt(i);
                            delete element;
                            return;  // Solo eliminamos un elemento a la vez
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
void MapEditor::updateTerrainUI(int terrainType) {
    try {
        // Se podría usar para cambiar colores, estilos o comportamientos específicos
        // según el tipo de terreno seleccionado
        switch (terrainType) {
            case 0:  // DESERT
                setWindowTitle("Counter-Strike Editor - Mapa Desierto");
                break;
            case 1:  // AZTEC_VILLAGE
                setWindowTitle("Counter-Strike Editor - Mapa Pueblito Azteca");
                break;
            case 2:  // TRAINING_ZONE
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

void MapEditor::generarMapaClicked() {
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

QString MapEditor::obtenerNombreArchivo() {
    bool ok;
    QString nombreArchivo =
            QInputDialog::getText(this, tr("Nombre del Mapa"), tr("Ingrese el nombre del mapa:"),
                                  QLineEdit::Normal, currentMapName, &ok);

    if (ok && !nombreArchivo.isEmpty()) {
        currentMapName = nombreArchivo;
        return nombreArchivo;
    } else {
        return QString();
    }
}

// Método para cargar un mapa
void MapEditor::loadMapClicked() {
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
        bool created = currentDir.mkpath(
                mapsPath);  // mkpath crea directorios recursivamente si es necesario
        qDebug() << "Creando directorio server/maps:" << (created ? "Éxito" : "Fallo");
    }

    // Obtener la ruta absoluta para mostrar en el diálogo
    QString absoluteMapsPath = currentDir.absoluteFilePath(mapsPath);
    qDebug() << "Directorio de mapas para cargar:" << absoluteMapsPath;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir Mapa"), absoluteMapsPath,
                                                    tr("Archivos YAML (*.yaml)"));

    if (!fileName.isEmpty()) {
        loadMapFromFile(fileName);
    }
}

// Método para cargar un mapa desde un archivo
void MapEditor::loadMapFromFile(const QString& fileName) {
    // Limpiar la escena actual
    QList<QGraphicsItem*> itemsToRemove;
    foreach (QGraphicsItem* item, scene->items()) {
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
    foreach (QGraphicsItem* item, itemsToRemove) {
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
        foreach (MapElement* element, elements) {
            // Si es un tile, procesarlo diferente
            if (element->getType() == TILE) {
                Tile* tile = dynamic_cast<Tile*>(element);
                if (tile && tile->getTileId() >= 0 && tilePixmaps.contains(tile->getTileId())) {
                    // Obtener la posición de la cuadrícula
                    QPoint gridPos = getTileGridPosition(tile->getPosition());

                    // Colocar el tile en esa posición
                    TileItem* tileItem =
                            new TileItem(tilePixmaps[tile->getTileId()], tile->getTileId(),
                                         QString("Tile_%1").arg(tile->getTileId()), scene);
                    tileItem->setPos(gridPos.x() * 32, gridPos.y() * 32);
                    tileItem->setZValue(-0.5);
                    scene->addItem(tileItem);

                    // Guardar la referencia del tile colocado
                    placedTiles[QPair<int, int>(gridPos.x(), gridPos.y())] = tile->getTileId();

                    qDebug() << "Cargando tile" << tile->getTileId() << "en posición" << gridPos.x()
                             << "," << gridPos.y();
                }
            } else {
                // Procesar otros elementos normalmente
                DragAndDrop* item = createDragAndDropItem(element);
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
bool MapEditor::validateMap() {
    QList<MapElement*> elements;

    // Debug: Imprimir qué tiles están colocados para diagnóstico
    qDebug() << "Validando mapa - Tiles colocados:";
    for (auto it = placedTiles.begin(); it != placedTiles.end(); ++it) {
        QPair<int, int> pos = it.key();
        int tileId = it.value();
        qDebug() << "  Tile ID:" << tileId << "en posición (" << pos.first << "," << pos.second
                 << ")";
    }

    // Convertir los elementos gráficos a elementos del mapa
    foreach (QGraphicsItem* item, scene->items()) {
        // Verificar si el ítem tiene datos válidos (en vez de usar dynamic_cast)
        if (item->data(1).isValid() && item->data(1).toInt() == TILE) {
            QPointF position = item->scenePos();
            int tileId = item->data(0).toInt();

            QPoint gridPos = getTileGridPosition(position);
            QPointF alignedPos(gridPos.x() * 32.0, gridPos.y() * 32.0);

            float x = MapEditor::pixelToWorldX(alignedPos.x(), 1.0);
            float y = MapEditor::pixelToWorldY(alignedPos.y(), 1.0);

            QPointF worldPos(x, y);
            qDebug() << "  Convirtiendo tile ID:" << tileId << "en posición:" << worldPos.x() << ","
                     << worldPos.y();

            Tile* tile = new Tile(worldPos, tileId);
            elements.append(tile);
        } else if (item->data(1).isValid() && item->data(1).toInt() == WEAPON) {
            QPointF position = item->scenePos();
            int weaponType = item->data(0).toInt();

            QPoint gridPos = getTileGridPosition(position);
            QPointF alignedPos(gridPos.x() * 32.0, gridPos.y() * 32.0);

            float x = MapEditor::pixelToWorldX(alignedPos.x(), 1.0);
            float y = MapEditor::pixelToWorldY(alignedPos.y(), 1.0);

            QPointF worldPos(x, y);

            Weapon* weapon = new Weapon(worldPos, weaponType);
            elements.append(weapon);
        }
        // Procesar QGraphicsPixmapItem (tiles)
        else if (QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item)) {
            // Verificar si es un tile (tiene los metadatos adecuados)
            if (pixmapItem->data(1).isValid() && pixmapItem->data(1).toInt() == TILE &&
                pixmapItem->data(0).isValid()) {
                int tileId = pixmapItem->data(0).toInt();
                QPointF position = pixmapItem->pos();

                // Convertir posición de pixeles a unidades de mundo
                QPoint gridPos = getTileGridPosition(position);
                QPointF alignedPos(gridPos.x() * 32.0, gridPos.y() * 32.0);

                float elementWidth = pixmapItem->pixmap().width() / 25.6;
                float elementHeight = pixmapItem->pixmap().height() / 25.6;
                float x = pixelToWorldX(alignedPos.x(), elementWidth);
                float y = pixelToWorldY(alignedPos.y(), elementHeight);

                QPointF worldPos(x, y);

                // Crear el elemento de mapa y añadirlo a la lista
                MapElement* element = new Tile(worldPos, tileId);
                elements.append(element);

                qDebug() << "Validando Tile ID:" << tileId << "en posición:" << worldPos.x() << ","
                         << worldPos.y();
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
void MapEditor::updateSelectionIndicator(QPointF scenePos) {
    // Obtener la posición de cuadrícula
    QPoint gridPos = getTileGridPosition(scenePos);

    // Verificar que esté dentro de los límites de la cuadrícula
    if (gridPos.x() >= 0 && gridPos.x() < 1000 / 32 && gridPos.y() >= 0 &&
        gridPos.y() < 1000 / 32) {
        // Calcular la posición exacta en la escena (alineada con la cuadrícula)
        qreal x = gridPos.x() * 32.0;
        qreal y = gridPos.y() * 32.0;

        // Actualizar la posición del indicador
        if (selectionIndicator) {
            selectionIndicator->setPos(x, y);
            selectionIndicator->show();  // Asegurar que sea visible
        }
    }
}

bool MapEditor::eventFilter(QObject* watched, QEvent* event) {
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
                event->type() != QEvent::MouseButtonRelease && event->type() != QEvent::MouseMove) {
                return QMainWindow::eventFilter(watched, event);
            }

            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QPointF scenePos = view->mapToScene(mouseEvent->pos());

            // Actualizar el indicador de selección para cualquier evento de mouse
            if (event->type() == QEvent::MouseMove) {
                updateSelectionIndicator(scenePos);
            }

            // MOUSE MOVE — pintar si se mantiene presionado el botón
            if (event->type() == QEvent::MouseMove) {
                updateSelectionIndicator(scenePos);

                if (mousePressed) {
                    // Colocar elemento según lo que esté seleccionado actualmente
                    if (currentTileId >= 0 && tilePixmaps.contains(currentTileId)) {
                        // Colocar un tile si está disponible en el mapa
                        placeTile(scenePos);
                        return true;
                    } else if (currentWeaponId >= 0 && weaponPixmaps.contains(currentWeaponId)) {
                        // Colocar un arma
                        placeWeapon(scenePos);
                        return true;
                    }
                }
            }

            // MOUSE PRESS — establecer flag
            if (event->type() == QEvent::MouseButtonPress &&
                mouseEvent->button() == Qt::LeftButton) {
                mousePressed = true;
            }

            // MOUSE RELEASE — limpiar flag
            if (event->type() == QEvent::MouseButtonRelease &&
                mouseEvent->button() == Qt::LeftButton) {
                mousePressed = false;
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
                    } else if (currentWeaponId >= 0 && weaponPixmaps.contains(currentWeaponId)) {
                        // Colocar un arma
                        placeWeapon(scenePos);
                        return true;
                    }
                } else if (mouseEvent->button() == Qt::RightButton) {
                    // Clic derecho para eliminar elementos
                    // Primero intentamos eliminar tiles en esa posición, independientemente de la
                    // herramienta
                    QPoint gridPos = getTileGridPosition(scenePos);
                    QPair<int, int> gridKey(gridPos.x(), gridPos.y());

                    // Verificar si hay un tile en la posición para eliminar (verificar tanto TILE
                    // como TILE_ELEMENT)
                    bool tileExists = false;
                    QList<QGraphicsItem*> itemsAtPos =
                            scene->items(QRectF(gridPos.x() * 32, gridPos.y() * 32, 32, 32));
                    for (QGraphicsItem* item: itemsAtPos) {
                        // Comprobar si es un tile, nos aseguramos de verificar ambos posibles tipos
                        if (item->data(1).isValid() && (item->data(1).toInt() == TILE_ELEMENT ||
                                                        item->data(1).toInt() == TILE)) {
                            tileExists = true;
                            qDebug() << "Tile encontrado para eliminar con ID:"
                                     << item->data(1).toInt();
                            break;
                        }
                    }

                    if (tileExists) {
                        // Si hay un tile, eliminarlo sin importar la herramienta seleccionada
                        removeTile(scenePos);
                        return true;
                    } else if (currentTileId >= 0) {
                        // Si no hay tile pero estamos en modo tile, intentar eliminar de todas
                        // formas
                        removeTile(scenePos);
                        return true;
                    } else if (currentWeaponId >= 0) {
                        // Para armas, intentar eliminar cualquier elemento en esa posición
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

// Método para precargar el mapa con un tile base del terreno actual
void MapEditor::preloadMapWithBaseTile() {
    // Si no hay tiles disponibles para el terreno actual, no hacer nada
    if (tilePixmaps.isEmpty()) {
        qDebug() << "No hay tiles disponibles para precargar el mapa";
        QMessageBox::warning(this, "Precargar Mapa",
                             "No hay tiles disponibles para el terreno seleccionado.");
        return;
    }

    // Preguntar confirmación al usuario
    QMessageBox::StandardButton reply =
            QMessageBox::question(this, "Precargar Mapa",
                                  "¿Desea llenar todo el mapa con el tile base del terreno "
                                  "actual?\n\nEsto eliminará todos los tiles existentes.",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Primero eliminamos todos los tiles existentes en el mapa
    for (QGraphicsItem* item: scene->items()) {
        if (TileItem* tileItem = dynamic_cast<TileItem*>(item)) {
            scene->removeItem(tileItem);
            delete tileItem;
        }
    }

    // Limpiamos el mapa de tiles colocados
    placedTiles.clear();

    // Determinar el tipo de terreno y obtener el primer tile del nuevo sistema
    QString terrainName;
    switch (currentTerrainType) {
        case 0:  // DESERT_TERRAIN
            terrainName = "Desert";
            break;
        case 1:  // AZTEC_VILLAGE
            terrainName = "Aztec";
            break;
        case 2:  // TRAINING_GROUND
            terrainName = "Training";
            break;
        default:
            terrainName = "Desert";
            break;
    }

    // Verificar que tenemos tiles disponibles en el mapa tilePixmaps
    if (tilePixmaps.size() == 0) {
        QMessageBox::warning(this, "Precargar Mapa",
                             "No hay tiles disponibles. Asegúrese de que los assets están "
                             "correctamente cargados.");
        return;
    }

    // Obtener el primer tile (ID 1) como tile base para precargar
    // Si no existe el ID 1, usar el primer tile disponible
    int baseTileId = 4;
    if (!tilePixmaps.contains(baseTileId)) {
        baseTileId = tilePixmaps.firstKey();
    }

    // Obtener el pixmap del tile base
    QPixmap basePixmap = tilePixmaps[baseTileId];

    qDebug() << "Precargando mapa con tile ID:" << baseTileId;

    // Dimensiones del mapa en tiles (considerando que el mapa es de 1000x1000 pixeles y cada tile
    // es de 32x32)
    int mapWidth = 1000 / 32;
    int mapHeight = 1000 / 32;

    // Primero, eliminar cualquier tile existente en la escena antes de la precarga
    // para asegurar que no hay conflictos
    QList<QGraphicsItem*> allItems = scene->items();
    for (QGraphicsItem* item: allItems) {
        // Solo eliminar items que sean TileItem pero no otros elementos como zonas
        TileItem* tileItem = dynamic_cast<TileItem*>(item);
        if (tileItem) {
            scene->removeItem(item);
            delete item;
        }
    }

    // Colocamos el tile base en todas las posiciones del mapa - alineados a la cuadrícula 32x32
    for (int x = 0; x < mapWidth; ++x) {
        for (int y = 0; y < mapHeight; ++y) {
            // Alineamos correctamente a la cuadrícula multiplicando por 32
            QPointF tilePos(x * 32, y * 32);

            // Crear tile con un ID válido del tileset actual
            QString tileName = QString("Tile_%1_%2").arg(x).arg(y);

            // Crear el tile con los parámetros correctos para que pueda ser manipulado
            // Usamos el pixmap y el ID del nuevo sistema de tiles
            TileItem* tileItem = new TileItem(basePixmap, baseTileId, tileName, scene);

            // Posicionarlo correctamente en la cuadrícula
            tileItem->setPos(tilePos);

            // Desactivar la capacidad de ser movido con drag and drop
            tileItem->setFlag(QGraphicsItem::ItemIsMovable, false);

            // Pero permitir que siga siendo seleccionable para eliminar con clic derecho
            tileItem->setFlag(QGraphicsItem::ItemIsSelectable, true);

            // Asegurarnos explícitamente de que el tile tenga el tipo correcto en setData
            // Esto garantiza que sea detectable por el código de eliminación
            tileItem->setData(1, QVariant(TILE));  // Usamos TILE (5) como lo espera removeTile

            // Añadir a la escena
            scene->addItem(tileItem);

            // Registrar en el mapa de tiles colocados para el correcto seguimiento
            placedTiles[qMakePair(x, y)] = baseTileId;

            // Debug para verificar colocación
            if ((x == 0 && y == 0) || (x == mapWidth - 1 && y == mapHeight - 1)) {
                qDebug() << "Tile colocado en:" << tilePos << "ID:" << baseTileId;
            }
        }
    }

    QMessageBox::information(this, "Precargar Mapa",
                             QString("Mapa precargado exitosamente con tile base de %1 (ID: %2).")
                                     .arg(terrainName)
                                     .arg(baseTileId));
}

void MapEditor::generateMapFile(const QString& fileName) {
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
        bool created =
                dir.mkpath(mapsDir);  // mkpath crea directorios recursivamente si es necesario
        qDebug() << "Creando directorio server/maps:" << (created ? "Éxito" : "Fallo");
    } else {
        qDebug() << "Directorio server/maps ya existe en:" << dir.absoluteFilePath(mapsDir);
    }

    // Actualizar el directorio donde se guardarán los mapas
    QString directorio = mapsDir;

    // Lista para almacenar todos los elementos
    QList<MapElement*> elements;

    // Convertir todos los elementos gráficos a elementos del mapa
    foreach (QGraphicsItem* item, scene->items()) {
        if (DragAndDrop* dragItem = dynamic_cast<DragAndDrop*>(item)) {
            MapElement* element = convertToMapElement(dragItem);
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
        MapElement* tileElement = new Tile(pixelPos, tileId);
        elements.append(tileElement);

        qDebug() << "Guardando tile" << tileId << "en posición" << gridPos.first << ","
                 << gridPos.second;
    }

    // Guardar el mapa en formato YAML
    bool success = YamlHandler::saveMapToYaml(directorio + fileName + ".yaml", elements,
                                              currentTerrainType, fileName);

    // Liberar memoria
    qDeleteAll(elements);
    elements.clear();

    if (success) {
        QMessageBox::information(
                this, "Guardado Exitoso",
                "El mapa ha sido guardado correctamente en " + directorio + fileName + ".yaml");
    } else {
        QMessageBox::critical(this, "Error",
                              "No se pudo guardar el mapa en " + directorio + fileName + ".yaml");
    }
}

// Convertir un elemento gráfico (DragAndDrop) a un elemento del mapa (MapElement)
MapElement* MapEditor::convertToMapElement(DragAndDrop* item) {
    if (!item)
        return nullptr;

    QPointF position = item->scenePos();
    // Usar data(1) para obtener el tipo de elemento, que es donde lo almacenamos al colocar
    // elementos
    int elementType = item->data(1).toInt();

    // Verificar que el tipo de elemento sea válido
    if (elementType < 0) {
        qDebug() << "Error: Tipo de elemento inválido o no definido:" << elementType;
        return nullptr;
    }

    // Convertir posición de pixeles a unidades de mundo si es necesario
    // Ajustar a la cuadrícula para asegurar alineación correcta
    QPoint gridPos = getTileGridPosition(position);
    QPointF alignedPos(gridPos.x() * 32.0, gridPos.y() * 32.0);

    float elementWidth = (item->pixmap().width()) / 25.6;
    float elementHeight = (item->pixmap().height()) / 25.6;
    float x = MapEditor::pixelToWorldX(alignedPos.x(), elementWidth);
    float y = MapEditor::pixelToWorldY(alignedPos.y(), elementHeight);

    QPointF worldPos(x, y);

    qDebug() << "Convirtiendo elemento tipo:" << elementType << "en posición:" << worldPos.x()
             << "," << worldPos.y();

    // Ahora solo manejamos TILE y WEAPON, los demás tipos han sido eliminados
    switch (elementType) {
        case TILE: {
            // Para tiles, usar data(0) para obtener el ID del tile
            int tileId = item->data(0).toInt();
            qDebug() << "  - Tile ID:" << tileId;
            return new Tile(worldPos, tileId);
        }

        case WEAPON: {
            // Usar data(0) para obtener el tipo de arma
            int weaponType = item->data(0).toInt();
            qDebug() << "  - Arma tipo:" << weaponType;
            return new Weapon(worldPos, weaponType);
        }

        default:
            qDebug() << "  - Tipo de elemento no reconocido:" << elementType;
            return nullptr;
    }
}

// Crear un elemento gráfico (DragAndDrop) a partir de un elemento del mapa (MapElement)
DragAndDrop* MapEditor::createDragAndDropItem(const MapElement* element) {
    if (!element)
        return nullptr;

    int elementType = element->getType();
    QPointF position = element->getPosition();

    // Convertir de unidades de mundo a pixeles si es necesario
    float x = MapEditor::worldToPixelX(position.x());
    float y = MapEditor::worldToPixelY(position.y());

    QPointF pixelPos(x, y);

    // Crear el elemento gráfico según su tipo
    QPixmap pixmap;
    DragAndDrop* item = nullptr;

    // Ahora solo manejamos TILE y WEAPON
    switch (elementType) {
        case TILE: {
            const Tile* tile = static_cast<const Tile*>(element);
            int tileId = tile->getTileId();
            if (tilePixmaps.contains(tileId)) {
                pixmap = tilePixmaps[tileId];
                item = new DragAndDrop(pixmap, 1.0, scene);
                item->setTipoElemento(TILE);
                item->setData(0, tileId);  // Guardar el ID del tile
            }
            break;
        }

        case WEAPON: {
            const Weapon* weapon = static_cast<const Weapon*>(element);
            int weaponType = weapon->getWeaponType();
            if (weaponPixmaps.contains(weaponType)) {
                pixmap = weaponPixmaps[weaponType];
                item = new DragAndDrop(pixmap, 0.6, scene);
                item->setTipoElemento(WEAPON);
                item->setData(0, weaponType);  // Guardar el tipo de arma
            }
            break;
        }
    }

    if (item) {
        item->setPos(pixelPos);
    }

    return item;
}
