#pragma once

#include <QButtonGroup>
#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QList>
#include <QMainWindow>
#include <QMap>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPair>
#include <QPixmap>
#include <QPointF>
#include <QPushButton>
#include <QScrollArea>
#include <QString>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWidget>

#include "backgroundItem.h"
#include "mapElements.h"
#include "mapValidator.h"
#include "tileItem.h"
#include "yamlHandler.h"

// Forward declaration
class DragAndDrop;

class MapEditor: public QMainWindow {

public:
    MapEditor(QWidget* parent = nullptr);
    ~MapEditor();  // Corregir la declaración duplicada del destructor

    // Event filter para capturar eventos de ratón
    bool eventFilter(QObject* watched, QEvent* event) override;
    void generateMapFile(const QString& nombreArchivo);
    QString obtenerNombreArchivo();
    bool validateMap();

private slots:
    // Ya no necesitamos estos métodos, ahora los elementos se colocan directamente con clicks

    // Selección de terreno
    void backgroundSelection(int index);

    // Control de zoom
    void zoomIn();
    void zoomOut();
    void applyZoom();

    // Sistema de tiles
    void tileSelected(int id);
    void placeTile(QPointF scenePos);
    // Se eliminó el método placeExtraTile
    void removeTile(QPointF scenePos);

    // Métodos para manipular armas
    void placeWeapon(QPointF scenePos);      // Colocar un arma en la posición del ratón
    void removeElementAt(QPointF scenePos);  // Eliminar cualquier elemento en la posición del ratón

    // Métodos de selección
    void weaponSelected(int id);

    // Método para actualizar el indicador de selección
    void updateSelectionIndicator(QPointF scenePos);

    // Método para actualizar el estilo del botón seleccionado
    void updateSelectedButtonStyle(QButtonGroup* buttonGroup, int selectedId);

    // Acciones de archivo
    void generarMapaClicked();
    void loadMapClicked();
    void loadMapFromFile(const QString& fileName);

    // Precargar mapa con tile base
    void preloadMapWithBaseTile();


private:
    // Recursos y escena
    BackgroundItem* currentBackground = nullptr;
    QGraphicsScene* scene;
    QGraphicsView* view;
    QGraphicsRectItem* selectionIndicator = nullptr;  // Indicador de selección
    int widthBeam = 0;
    int currentTerrainType = 0;
    QString currentMapName;
    float currentZoomFactor = 1.5;  // Factor de zoom actual

    // Sistema de tiles
    QMap<int, QPixmap> tilePixmaps;          // Almacena todos los tiles disponibles
    QMap<QPair<int, int>, int> placedTiles;  // Tiles colocados en formato (x,y) -> tileId
    // Ya no usamos tileMap, los tiles ahora son QGraphicsPixmapItem simples
    int currentTileId = -1;        // ID del tile seleccionado actualmente
    QScrollArea* tilesScrollArea;  // Área de desplazamiento para la paleta de tiles
    QGroupBox* tilesGroup;         // Contenedor para la paleta de tiles
    QButtonGroup* tileButtons;     // Grupo de botones para los tiles

    // Se eliminaron las variables para sólidos, zonas, zonas de bomba y extra-tiles

    // Sistema de armas
    QMap<int, QPixmap> weaponPixmaps;  // Almacena todas las armas disponibles
    int currentWeaponId = -1;          // ID del arma seleccionada actualmente
    QScrollArea* weaponsScrollArea;    // Área de desplazamiento para la paleta de armas
    QGroupBox* weaponsGroup;           // Contenedor para la paleta de armas
    QButtonGroup* weaponButtons;       // Grupo de botones para las armas

    // Lista de elementos del mapa (no tiles)
    QList<MapElement*> mapElements{};  // Lista para almacenar todos los elementos del mapa

    // Elementos de la UI
    QTabWidget* elementsTabWidget;  // Widget de pestañas para los diferentes elementos
    QComboBox* terrainCombo;        // Selector de tipo de terreno
    QWidget* toolPanel;             // Panel de herramientas lateral
    QVBoxLayout* toolPanelLayout;   // Layout para el panel de herramientas

    bool mousePressed = false;

    // Método para obtener la ruta base a los recursos
    static QString getResourcesPath();

    // Método para cortar una imagen de tileset en tiles individuales
    QMap<int, QPixmap> sliceTilesetImage(const QString& tilesetPath, int tileWidth, int tileHeight);

    // Métodos privados
    QString getResourcePath(int elementType, int subType = 0);
    void updateTerrainUI(int terrainType);
    QColor getTerrainColor(int terrainType);

    // Funciones de escala para convertir entre coordenadas de píxeles y mundo
    static float pixelToWorldX(float pixelX, float elementWidth = 0.0f);
    static float pixelToWorldY(float pixelY, float elementHeight = 0.0f);
    static float worldToPixelX(float worldX);
    static float worldToPixelY(float worldY);

    // Convertir elementos gráficos a elementos del mapa
    MapElement* convertToMapElement(DragAndDrop* item);

    // Crear elementos gráficos a partir de elementos del mapa
    DragAndDrop* createDragAndDropItem(const MapElement* element);

    // Sistema de tiles y otros elementos
    void loadElementsFromPath(const QString& path, QMap<int, QPixmap>& pixmapMap,
                              QButtonGroup* buttonGroup, QScrollArea* scrollArea,
                              void (MapEditor::*selectCallback)(int));
    void loadAvailableTiles();    // Cargar los tiles disponibles según el terreno
    void loadAvailableWeapons();  // Cargar las armas

    // Métodos de selección de elementos
    // Las funciones de selección están definidas más arriba

    void createTilesPanel();                              // Crear el panel de selección de tiles
    QPoint getTileGridPosition(const QPointF& scenePos);  // Convertir posición de la escena a
                                                          // coordenadas de cuadrícula
};
