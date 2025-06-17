#pragma once

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QWidget>
#include <QComboBox>
#include <QString>
#include <QPointF>
#include <QDebug>
#include <QTextStream>
#include <QDir>
#include <QInputDialog>
#include <QList>
#include <QFileDialog>
#include <QMessageBox>
#include <QMap>
#include <QScrollArea>
#include <QPushButton>
#include <QButtonGroup>
#include <QMouseEvent>
#include <QPair>
#include "backgroundItem.h"
#include "mapElements.h"
#include "yamlHandler.h"
#include "mapValidator.h"
#include "tileItem.h"

// Forward declaration
class DragAndDrop;

class MapEditor : public QMainWindow
{

public:
    MapEditor(QWidget *parent = nullptr);
    ~MapEditor(); // Corregir la declaración duplicada del destructor
    
    // Event filter para capturar eventos de ratón
    bool eventFilter(QObject* watched, QEvent* event) override;
    void generateMapFile(const QString &nombreArchivo);
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
    void placeExtraTile(QPointF scenePos); // Colocar un tile extra
    void removeTile(QPointF scenePos);
    
    // Métodos para manipular sólidos, zonas y armas
    void placeSolid(QPointF scenePos); // Colocar un sólido en la posición del ratón
    void placeZone(QPointF scenePos); // Colocar una zona en la posición del ratón
    void placeBombZone(QPointF scenePos); // Colocar una zona de bomba en la posición del ratón
    void placeWeapon(QPointF scenePos); // Colocar un arma en la posición del ratón
    void removeElementAt(QPointF scenePos); // Eliminar cualquier elemento en la posición del ratón
    
    // Métodos de selección
    void zoneSelected(int id);
    void solidSelected(int id);
    void weaponSelected(int id);
    void bombZoneSelected(int id); // Seleccionar zona de bomba
    void extraTileSelected(int id); // Seleccionar tile extra
    
    // Método para actualizar el indicador de selección
    void updateSelectionIndicator(QPointF scenePos);
    
    // Método para actualizar el estilo del botón seleccionado
    void updateSelectedButtonStyle(QButtonGroup* buttonGroup, int selectedId);
    
    // Acciones de archivo
    void generarMapaClicked();
    void loadMapClicked();
    void loadMapFromFile(const QString &fileName);
    
    // Precargar mapa con tile base
    void preloadMapWithBaseTile();


private:
    // Recursos y escena
    BackgroundItem *currentBackground = nullptr;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsRectItem* selectionIndicator = nullptr; // Indicador de selección
    int widthBeam = 0;
    int currentTerrainType = 0;
    QString currentMapName;
    float currentZoomFactor = 1.5; // Factor de zoom actual
    
    // Sistema de tiles
    QMap<int, QPixmap> tilePixmaps; // Almacena todos los tiles disponibles
    QMap<QPair<int, int>, int> placedTiles; // Tiles colocados en formato (x,y) -> tileId
    // Ya no usamos tileMap, los tiles ahora son QGraphicsPixmapItem simples
    int currentTileId = -1; // ID del tile seleccionado actualmente
    QScrollArea* tilesScrollArea; // Área de desplazamiento para la paleta de tiles
    QGroupBox* tilesGroup; // Contenedor para la paleta de tiles
    QButtonGroup* tileButtons; // Grupo de botones para los tiles
    
    // Sistema de sólidos
    QMap<int, QPixmap> solidPixmaps; // Almacena todos los sólidos disponibles
    int currentSolidId = -1; // ID del sólido seleccionado actualmente
    QScrollArea* solidsScrollArea; // Área de desplazamiento para la paleta de sólidos
    QGroupBox* solidsGroup; // Contenedor para la paleta de sólidos
    QButtonGroup* solidButtons; // Grupo de botones para los sólidos
    
    // Sistema de zonas
    QMap<int, QPixmap> zonePixmaps; // Almacena todas las zonas disponibles
    int currentZoneId = -1; // ID de la zona seleccionada actualmente
    QScrollArea* zonesScrollArea; // Área de desplazamiento para la paleta de zonas
    QGroupBox* zonesGroup; // Contenedor para la paleta de zonas
    QButtonGroup* zoneButtons; // Grupo de botones para las zonas
    
    // Sistema de zonas de bomba
    QMap<int, QPixmap> bombZonePixmaps; // Almacena todas las zonas de bomba disponibles
    int currentBombZoneId = -1; // ID de la zona de bomba seleccionada actualmente
    QScrollArea* bombZonesScrollArea; // Área de desplazamiento para la paleta de zonas de bomba
    QGroupBox* bombZonesGroup; // Contenedor para la paleta de zonas de bomba
    QButtonGroup* bombZoneButtons; // Grupo de botones para las zonas de bomba
    
    // Sistema de extra-tiles
    QMap<int, QPixmap> extraTilePixmaps; // Almacena todos los tiles extra disponibles
    int currentExtraTileId = -1; // ID del tile extra seleccionado actualmente
    QScrollArea* extraTilesScrollArea; // Área de desplazamiento para la paleta de tiles extra
    QGroupBox* extraTilesGroup; // Contenedor para la paleta de tiles extra
    QButtonGroup* extraTileButtons; // Grupo de botones para los tiles extra
    
    // Sistema de armas
    QMap<int, QPixmap> weaponPixmaps; // Almacena todas las armas disponibles
    int currentWeaponId = -1; // ID del arma seleccionada actualmente
    QScrollArea* weaponsScrollArea; // Área de desplazamiento para la paleta de armas 
    QGroupBox* weaponsGroup; // Contenedor para la paleta de armas
    QButtonGroup* weaponButtons; // Grupo de botones para las armas
    
    // Lista de elementos del mapa (no tiles)
    QList<MapElement*> mapElements{}; // Lista para almacenar todos los elementos del mapa
    
    // Elementos de la UI
    QTabWidget* elementsTabWidget; // Widget de pestañas para los diferentes elementos
    QComboBox* terrainCombo;     // Selector de tipo de terreno
    QWidget* toolPanel;          // Panel de herramientas lateral
    QVBoxLayout* toolPanelLayout;   // Layout para el panel de herramientas
    
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
    void loadAvailableTiles(); // Cargar los tiles disponibles según el terreno
    void loadAvailableSolids(); // Cargar los elementos sólidos
    void loadAvailableZones(); // Cargar las zonas
    void loadAvailableBombZones(); // Cargar las zonas de bomba
    void loadAvailableExtraTiles(); // Cargar los tiles extra
    void loadAvailableWeapons(); // Cargar las armas
    
    // Métodos de selección de elementos
    // Las funciones de selección están definidas más arriba
    
    void createTilesPanel(); // Crear el panel de selección de tiles
    QPoint getTileGridPosition(const QPointF& scenePos); // Convertir posición de la escena a coordenadas de cuadrícula
};
