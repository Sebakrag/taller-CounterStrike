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
    // Elementos del mapa
    void addBox();
    void addSpawn();
    void addBombZone();
    void addWeapon(int weaponType = 0);
    void addTeamSpawnCT();
    void addTeamSpawnT();
    
    // Selección de terreno
    void backgroundSelection(int index);
    
    // Control de zoom
    void zoomIn();
    void zoomOut();
    void applyZoom();
    
    // Sistema de tiles
    void tileSelected(int id);
    void placeTile(QPointF scenePos);
    void removeTile(QPointF scenePos);
    
    // Acciones de archivo
    void generarMapaClicked();
    void loadMapClicked();
    void loadMapFromFile(const QString &fileName);


private:
    // Recursos y escena
    BackgroundItem *currentBackground;
    QGraphicsScene *scene;
    QGraphicsView *view;
    int widthBeam = 0;
    int currentTerrainType = 0;
    QString currentMapName;
    float currentZoomFactor = 1.5; // Factor de zoom actual
    
    // Sistema de tiles
    QMap<int, QPixmap> tilePixmaps; // Almacena todos los tiles disponibles
    QMap<QPair<int, int>, int> placedTiles; // Tiles colocados en formato (x,y) -> tileId
    int currentTileId = -1; // ID del tile seleccionado actualmente
    QScrollArea* tilesScrollArea; // Área de desplazamiento para la paleta de tiles
    QGroupBox* tilesGroup; // Contenedor para la paleta de tiles
    QButtonGroup* tileButtons; // Grupo de botones para los tiles
    
    // Método para obtener la ruta base a los recursos
    static QString getResourcesPath();
    
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
    
    // Métodos para el sistema de tiles
    void loadAvailableTiles(); // Cargar los tiles disponibles en gfx/tiles
    void createTilesPanel(); // Crear el panel de selección de tiles
    QPoint getTileGridPosition(const QPointF& scenePos); // Convertir posición de la escena a coordenadas de cuadrícula
};
