#ifndef TILE_H
#define TILE_H

#include <cstdint>
#include <vector>

// Convención.
// los ids del 1 al 12 corresponden a tiles SÓLIDOS. Los posteriores son traspasables.
#define ID_MAX_SOLID 12

// 32x32 px
class Tile {
private:
    int id_tile;
    bool is_solid;  // dato que le sirve al server

public:
    explicit Tile(int id_tile);
    explicit Tile(int id_tile, bool is_solid);

    bool isSolid() const;

    int getIdTile() const;
};

#endif  // TILE_H
