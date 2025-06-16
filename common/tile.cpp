#include "tile.h"


Tile::Tile(int id_tile): id_tile(id_tile) {
    if (id_tile <= ID_MAX_SOLID) {
        is_solid = true;
    } else {
        is_solid = false;
    }
}

Tile::Tile(int id_tile, bool is_solid): id_tile(id_tile), is_solid(is_solid) {}

Tile::Tile(int id_tile, bool is_solid, bool is_bomb_zone) : id_tile(id_tile), is_solid(is_solid), is_bomb_zone(is_bomb_zone) {}

bool Tile::isSolid() const { return is_solid; }

bool Tile::isBombZone() const { return is_bomb_zone; }


int Tile::getIdTile() const { return id_tile; }
