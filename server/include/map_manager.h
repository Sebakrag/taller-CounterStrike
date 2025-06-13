#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "../../common/tile_map.h"

/**
 * Estructura para almacenar información básica de un mapa
 * disponible para seleccionar en una partida
 */
class MapInfo {
public:
    std::string name;
    std::string fileName;
    TypeTileMap type;
    int width;
    int height;

    MapInfo() : type(TypeTileMap::Desert), width(0), height(0) {}
};

/**
 * Gestor de mapas para el servidor
 * Se encarga de leer los mapas disponibles desde una carpeta
 * y convertirlos a TileMap para su uso en el juego
 */
class MapManager {
public:
    /**
     * Constructor
     * @param mapsFolderPath Ruta a la carpeta donde se encuentran los mapas YAML
     */
    explicit MapManager(const std::string& mapsFolderPath);
    
    /**
     * Actualiza la lista de mapas disponibles (lee la carpeta)
     */
    void updateAvailableMaps();
    
    /**
     * Obtiene la lista de mapas disponibles
     * @return Vector con la información de los mapas disponibles
     */
    std::vector<MapInfo> getAvailableMaps() const;
    
    /**
     * Carga un mapa específico y lo convierte a TileMap
     * @param fileName Nombre del archivo del mapa a cargar
     * @return TileMap cargado desde el archivo
     */
    TileMap loadMap(const std::string& fileName) const;

private:
    std::string mapsFolderPath;
    std::vector<MapInfo> availableMaps;
    
    /**
     * Extrae información básica de un archivo YAML
     * @param filePath Ruta al archivo YAML
     * @return Información básica del mapa
     */
    MapInfo getMapInfo(const std::string& filePath) const;
    
    /**
     * Convierte un nodo YAML a TileMap
     * @param mapNode Nodo YAML con la información del mapa
     * @return TileMap creado a partir del YAML
     */
    TileMap convertYamlToTileMap(const YAML::Node& mapNode) const;
};

#endif // MAP_MANAGER_H
