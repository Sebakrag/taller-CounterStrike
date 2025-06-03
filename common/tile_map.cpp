#include "tile_map.h"

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <random>

#include <netinet/in.h>


TileMap::TileMap(const std::vector<std::vector<int>>& matrizIdsTiles) {
    row_count = matrizIdsTiles[0].size();
    col_count = matrizIdsTiles.size();

    for (int i = 0; i < row_count; i++) {
        std::vector<Tile> row;
        for (int j = 0; j < col_count; j++) {
            row.push_back(Tile(matrizIdsTiles[j][i]));
        }
        matriz.push_back(row);
    }
}

TileMap::TileMap(const std::vector<uint8_t>& buffer) {
    int size = buffer.size();
    if (size < 4) {
        throw std::runtime_error("ERROR. No se pudo construir el TileMap."
                                 "El buffer no corresponde a un TileMap válido.");
    }

    uint16_t rows;
    std::memcpy(&rows, &buffer[0], sizeof(uint16_t));
    row_count = ntohs(rows);

    uint16_t cols;
    std::memcpy(&cols, &buffer[2], sizeof(uint16_t));
    col_count = ntohs(cols);

    if (size != row_count * col_count + 4) {
        throw std::runtime_error("ERROR. No se pudo construir el TileMap."
                                 "El buffer no corresponde a un TileMap válido.");
    }

    int index = 4;

    for (int i = 0; i < row_count; i++) {
        std::vector<Tile> row;
        for (int j = 0; j < col_count; j++) {
            row.push_back(Tile(buffer[index]));
            index++;
        }
        matriz.push_back(row);
    }
}

std::vector<uint8_t> TileMap::toBytes() {
    std::vector<uint8_t> buffer;

    // cargo cant. de filas
    uint16_t row_count_be = htons(row_count);  // paso a big endian
    uint8_t* bytes_be_ptr = reinterpret_cast<uint8_t*>(&row_count_be);
    buffer.push_back(bytes_be_ptr[0]);
    buffer.push_back(bytes_be_ptr[1]);

    // cargo cant. de columnas
    uint16_t col_count_be = htons(col_count);  // paso a big endian
    uint8_t* bytes_be_ptr_ = reinterpret_cast<uint8_t*>(&col_count_be);
    buffer.push_back(bytes_be_ptr_[0]);
    buffer.push_back(bytes_be_ptr_[1]);

    // cargo ids
    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < col_count; j++) {
            buffer.push_back((uint8_t)matriz[i][j].getIdTile());
        }
    }
    return buffer;
}

int TileMap::getIdTile(unsigned int row, unsigned int col) { return matriz[row][col].getIdTile(); }

Tile TileMap::getTile(unsigned int row, unsigned int col) { return matriz[col][row]; }

int TileMap::getRowCount() const { return row_count; }

int TileMap::getColCount() const { return col_count; }

void TileMap::print() {
    for (int j = 0; j < col_count; j++) {
        for (int i = 0; i < row_count; i++) {
            std::cout << std::to_string(matriz[i][j].getIdTile()) << " ";
        }
        std::cout << std::endl;
    }
}


int level_1[5][6] = {{2, 2, 2, 2, 2, 2},
                     {2, 1, 1, 1, 1, 2},
                     {2, 1, 2, 2, 1, 2},
                     {2, 1, 1, 1, 1, 2},
                     {2, 2, 2, 2, 2, 2}};

/*TileMap TileMap::getLevelDemo() {
    std::vector<std::vector<int>> matriz_ids_tiles;
    for (int i = 0; i < 5; ++i) {
        std::vector<int> fila;
        for (int j = 0; j < 6; ++j) {
            fila.push_back(level_1[i][j]);
        }
        matriz_ids_tiles.push_back(fila);
    }
    return TileMap(matriz_ids_tiles);
}*/

TileMap TileMap::getCurrentLevel() {
    constexpr int w = 1000;
    constexpr int h = 1000;
    std::vector<std::vector<int>> tileMap(h, std::vector<int>(w));

    std::random_device rd;  // fuente de entropía
    std::mt19937 gen(rd()); // motor de generación
    std::uniform_int_distribution<> dist(1, 46);  // distribución de IDs

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            tileMap[y][x] = dist(gen);
        }
    }

    return TileMap(tileMap);
}
