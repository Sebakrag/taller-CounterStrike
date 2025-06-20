#ifndef TILE_H
#define TILE_H

#include <cstdint>
#include <vector>

enum TypeTile { None, Solid, BombZone, T_Zone, CT_Zone };


// 32x32 px
class Tile {
private:
    int id_tile;
    TypeTile type = None;

public:
    explicit Tile(int id_tile);
    // explicit Tile(int id_tile, bool is_solid);
    explicit Tile(int id_tile, const TypeTile type);

    bool isSolid() const;
    bool isBombZone() const;
    bool isTerroristZone() const;
    bool isCounterTerroristZone() const;

    int getIdTile() const;
};

#endif  // TILE_H
