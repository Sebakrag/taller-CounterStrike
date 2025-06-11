#include "tile_map.h"

#include <cstring>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

#include <netinet/in.h>

#include "protocol.h"


TileMap::TileMap(const TypeTileMap& type, const std::vector<std::vector<int>>& matrizIdsTiles):
        type(type) {
    col_count = matrizIdsTiles[0].size();
    row_count = matrizIdsTiles.size();

    for (int i = 0; i < row_count; i++) {
        std::vector<Tile> row;
        for (int j = 0; j < col_count; j++) {
            row.push_back(Tile(matrizIdsTiles[i][j]));
        }
        matriz.push_back(row);
    }
}
TileMap::TileMap(const TileMap& other):
        type(other.type),
        matriz(other.matriz),
        row_count(other.row_count),
        col_count(other.col_count) {}

TileMap& TileMap::operator=(const TileMap& other) {
    if (this == &other)
        return *this;

    this->type = other.type;
    this->row_count = other.row_count;
    this->col_count = other.col_count;
    this->matriz = other.matriz;

    return *this;
}

TileMap::TileMap(const std::vector<uint8_t>& buffer) {
    int size = buffer.size();
    if (size < 9) {
        throw std::runtime_error("ERROR. No se pudo construir el TileMap."
                                 "El buffer no corresponde a un TileMap válido.");
    }
    type = Protocol_::decodeTypeTileMap(buffer[0]);

    row_count = Protocol_::getBigEndian32(buffer[1], buffer[2], buffer[3], buffer[4]);
    col_count = Protocol_::getBigEndian32(buffer[5], buffer[6], buffer[7], buffer[8]);

    if (size != row_count * col_count + 9) {
        throw std::runtime_error("ERROR. No se pudo construir el TileMap."
                                 "El buffer no corresponde a un TileMap válido.");
    }

    int index = 9;

    for (int i = 0; i < row_count; i++) {
        std::vector<Tile> row;
        for (int j = 0; j < col_count; j++) {
            row.push_back(Tile(buffer[index]));
            index++;
        }
        matriz.push_back(row);
    }
}

std::vector<uint8_t> TileMap::toBytes() const {
    std::vector<uint8_t> buffer;

    buffer.push_back(Protocol_::encodeTypeTileMap(type));
    // cargo cant. de filas (4 bytes)
    Protocol_::insertBigEndian32(row_count, buffer);

    // cargo cant. de columnas (4 bytes)
    Protocol_::insertBigEndian32(col_count, buffer);

    // cargo ids
    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < col_count; j++) {
            buffer.push_back((uint8_t)matriz[i][j].getIdTile());
        }
    }
    return buffer;
}

int TileMap::getIdTile(unsigned int row, unsigned int col) const {
    return matriz[row][col].getIdTile();
}

Tile TileMap::getTile(unsigned int row, unsigned int col) const { return matriz[row][col]; }

int TileMap::getRowCount() const { return row_count; }

int TileMap::getColCount() const { return col_count; }

TypeTileMap TileMap::getType() const { return type; }

void TileMap::print() const {
    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < col_count; j++) {
            std::cout << std::to_string(matriz[i][j].getIdTile()) << " ";
        }
        std::cout << std::endl;
    }
}

const std::vector<std::vector<int>> arena_desierto1 = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1,  13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
         13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 1,  1},
        {1,  13, 13, 13, 13, 13, 13, 13, 13, 21, 18, 18, 18, 23, 13,
         13, 21, 18, 18, 18, 23, 13, 13, 13, 13, 13, 13, 13, 13, 1},
        {1,  2,  13, 13, 2, 13, 13, 13, 13, 19, 3,  3,  3,  17, 13,
         13, 19, 3,  3,  3, 17, 13, 13, 13, 13, 13, 13, 13, 13, 1},
        {1,  2,  13, 13, 2, 2,  2,  13, 13, 19, 3,  3,  39, 24, 13,
         13, 22, 38, 3,  3, 17, 13, 13, 13, 13, 13, 13, 13, 13, 1},
        {1,  2,  13, 15, 13, 13, 2,  13, 13, 19, 3, 3, 17, 13, 13,
         13, 13, 19, 3,  3,  17, 13, 13, 2,  2,  2, 2, 2,  2,  1},
        {1,  2,  13, 13, 13, 13, 2, 13, 2, 34, 45, 45, 36, 13, 13,
         13, 13, 34, 45, 45, 36, 2, 13, 2, 13, 13, 13, 13, 2,  1},
        {1,  2,  2,  2, 2, 2,  2,  13, 13, 19, 3,  3,  17, 13, 13,
         13, 13, 19, 3, 3, 17, 13, 13, 2,  13, 13, 16, 13, 2,  1},
        {1,  13, 13, 13, 13, 13, 13, 13, 13, 19, 3, 3,  40, 23, 13,
         13, 21, 37, 3,  3,  17, 13, 13, 2,  2,  2, 13, 13, 2,  1},
        {1,  13, 13, 13, 13, 13, 13, 13, 13, 19, 3, 3,  3,  17, 13,
         13, 19, 3,  3,  3,  17, 13, 13, 13, 13, 2, 13, 13, 2,  1},
        {1,  13, 13, 13, 13, 13, 13, 13, 13, 22, 20, 20, 20, 24, 13,
         13, 22, 20, 20, 20, 24, 13, 13, 13, 13, 13, 13, 13, 13, 1},
        {1,  1,  13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
         13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};


TileMap TileMap::getLevelDemo() {
    return TileMap(TypeTileMap::Desert, arena_desierto1);  // si solo necesitas devolverlo tal cual
}

TileMap TileMap::getCurrentLevel() {
    constexpr int w = 1000;
    constexpr int h = 1000;
    std::vector<std::vector<int>> tileMap(h, std::vector<int>(w));

    std::random_device rd;                        // fuente de entropía
    std::mt19937 gen(rd());                       // motor de generación
    std::uniform_int_distribution<> dist(1, 46);  // distribución de IDs

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            tileMap[y][x] = dist(gen);
        }
    }

    return TileMap(TypeTileMap::Aztec, tileMap);
}
