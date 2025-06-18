#include "../include/map.h"

#include <iostream>


Map::Map(const TileMap& tilemap): 
        tileMap(tilemap), 
        posZoneTerrorist(tilemap.getPosTerroristZone()), 
        posZoneCounterTerrorist(tilemap.getPosCounterTerroistZone()) {}

bool Map::isWalkable(int x, int y) {
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;

    if (tileX < 0 || tileX >= tileMap.getColCount() || tileY < 0 ||
        tileY >= tileMap.getRowCount()) {
        return false;
    }

    return !tileMap.getTile(tileY, tileX).isSolid();
}

bool Map::isBombZone(int x, int y) {
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;

    if (tileX < 0 || tileX >= tileMap.getColCount() || tileY < 0 ||
        tileY >= tileMap.getRowCount()) {
        return false;
        }

    return tileMap.getTile(tileY, tileX).isBombZone();
}

Vec2D Map::getPositionTZone() {
    return posZoneTerrorist;
}

Vec2D Map::getPositionCTZone() {
    return posZoneCounterTerrorist;
}
