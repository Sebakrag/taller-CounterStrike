#ifndef MAP_INFO_H_
#define MAP_INFO_H_

#include <string>
#include "../../common/tile_map.h"

/**
 * Estructura para almacenar información básica de un mapa
 * disponible para seleccionar en una partida
 */
class ClientMapInfo {
public:
    std::string name;
    std::string fileName;
    TypeTileMap type;
    int width;
    int height;

    ClientMapInfo() : type(TypeTileMap::Desert), width(0), height(0) {}
    
    ClientMapInfo(const std::string& name, const std::string& fileName, 
               TypeTileMap type, int width, int height) 
        : name(name), fileName(fileName), type(type), width(width), height(height) {}
};

#endif // MAP_INFO_H_
