#include "tile_map.h"

#include <cstring>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

#include <netinet/in.h>

#include "utils/Vec2D.h"

#include "protocol.h"
#include "tile.h"

// constructor simple, para el cliente
TileMap::TileMap(const TypeTileMap& type, const std::vector<std::vector<int>>& matrizIdsTiles):
        type(type), pos_bomb_zone(0, 0), pos_t_zone(0, 0), pos_ct_zone(0, 0) {
    col_count = matrizIdsTiles[0].size();
    row_count = matrizIdsTiles.size();

    for (int i = 0; i < row_count; i++) {
        std::vector<Tile> row;
        for (int j = 0; j < col_count; j++) {
            row.emplace_back(Tile(matrizIdsTiles[i][j]));
        }
        matriz.push_back(std::move(row));
    }
}

// constructor con tipos de tile y posiciones de armas
TileMap::TileMap(const TypeTileMap& type, const std::vector<std::vector<int>>& matrizIdsTiles,
                 const std::map<int, TypeTile>& typesTiles,
                 const std::map<Vec2D, Weapon>& weaponPositions):
        type(type), weaponPositions(weaponPositions) {
    row_count = matrizIdsTiles.size();
    col_count = matrizIdsTiles[0].size();

    for (int i = 0; i < row_count; ++i) {
        std::vector<Tile> row;
        for (int j = 0; j < col_count; ++j) {
            int id = matrizIdsTiles[i][j];
            auto it = typesTiles.find(id);
            if (it == typesTiles.end()) {
                row.emplace_back(Tile(id));
            } else {
                TypeTile typeTyle = it->second;
                row.emplace_back(id, typeTyle);
                Vec2D pos_in_world(j * TILE_SIZE + 16, i * TILE_SIZE + 16);
                if (typeTyle == BombZone)
                    pos_bomb_zone = pos_in_world;
                else if (typeTyle == T_Zone)
                    pos_t_zone = pos_in_world;
                else if (typeTyle == CT_Zone)
                    pos_ct_zone = pos_in_world;
            }
        }
        matriz.push_back(std::move(row));
    }
}

TileMap::TileMap(const TileMap& other):
        type(other.type),
        matriz(other.matriz),
        row_count(other.row_count),
        col_count(other.col_count),
        pos_bomb_zone(other.pos_bomb_zone),
        pos_t_zone(other.pos_t_zone),
        pos_ct_zone(other.pos_ct_zone),
        weaponPositions(other.weaponPositions) {}

TileMap& TileMap::operator=(const TileMap& other) {
    if (this != &other) {
        type = other.type;
        row_count = other.row_count;
        col_count = other.col_count;
        matriz = other.matriz;
        pos_bomb_zone = other.pos_bomb_zone;
        pos_t_zone = other.pos_t_zone;
        pos_ct_zone = other.pos_ct_zone;
        weaponPositions = other.weaponPositions;
    }
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

Vec2D TileMap::getPosBombZone() const { return pos_bomb_zone; }
Vec2D TileMap::getPosTerroristZone() const { return pos_t_zone; }
Vec2D TileMap::getPosCounterTerroistZone() const { return pos_ct_zone; }

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
    std::map<int, TypeTile> typesTiles;
    for (int i = 0; i < 12; i++) {
        typesTiles[i] = Solid;
    }
    typesTiles[15] = T_Zone;
    typesTiles[16] = CT_Zone;
    return TileMap(TypeTileMap::Desert, arena_desierto1, typesTiles);
}
