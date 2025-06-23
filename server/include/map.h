#ifndef MAP_H
#define MAP_H

#include <vector>
#include <map>

#include "../../common/tile_map.h"

class Map {
private:
    TileMap tileMap;
    Vec2D posZoneTerrorist;
    Vec2D posZoneCounterTerrorist;

public:
    explicit Map(const TileMap& tilemap);
    bool isWalkable(int x, int y);
    bool isBombZone(int x, int y);

    Vec2D getPositionTZone();
    Vec2D getPositionCTZone();
    
    // Método para obtener las posiciones de armas del TileMap
    const std::map<Vec2D, Weapon>& getWeaponPositions() const;
};

#endif
