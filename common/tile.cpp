#include "tile.h"


Tile::Tile(int id_tile): id_tile(id_tile) { type = None; }

Tile::Tile(int id_tile, const TypeTile _type): id_tile(id_tile) {
    type = _type;
}

bool Tile::isSolid() const { return type == Solid; }
bool Tile::isBombZone() const { return type == BombZone; }
bool Tile::isTerroristZone() const { return type == T_Zone; }
bool Tile::isCounterTerroristZone() const { return type == CT_Zone; }

int Tile::getIdTile() const { return id_tile; }
