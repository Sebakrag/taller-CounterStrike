#include "tile.h"


Tile::Tile(int id_tile): id_tile(id_tile) {
    if (id_tile == ID_WALL1) {
        is_solid = true;
    } else {
        is_solid = false;
    }
}

Tile::Tile(int id_tile, bool is_solid): id_tile(id_tile), is_solid(is_solid) {}

bool Tile::isSolid() const { return is_solid; }

int Tile::getIdTile() const { return id_tile; }
