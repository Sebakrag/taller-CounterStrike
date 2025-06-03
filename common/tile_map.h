#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <cstdint>
#include <vector>

#include "tile.h"

class TileMap {
private:
    std::vector<std::vector<Tile>> matriz;  // matriz de tiles
    int row_count;
    int col_count;

public:
    TileMap();

    // Constructor a partir de una matriz de id's de tiles
    explicit TileMap(const std::vector<std::vector<int>>& matrizIdsTiles);

    /**
     * Constructor a partir de una tira de bytes, donde
     * los primeros 2 bytes indican la cantidad de filas
     * y los siguientes 2 bytes la cantidad de columnas
     */
    explicit TileMap(const std::vector<uint8_t>& buffer);

    /**
     * Serializa el tilemap en una tira de bytes,
     * donde los primeros 2 bytes indican la cantidad de filas,
     * y los siguientes 2 bytes la cantidad de columnas.
     */
    std::vector<uint8_t> toBytes();

    int getIdTile(unsigned int row, unsigned int col);

    Tile getTile(unsigned int row, unsigned int col);

    int getRowCount() const;
    int getColCount() const;

    void print();  // para testear

    //static TileMap getLevelDemo();
    static TileMap getCurrentLevel();
};


#endif  // TILE_MAP_H
