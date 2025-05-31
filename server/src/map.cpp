#include "../include/map.h"

#include <iostream>

Map::Map() {
    map = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
            {0, 0, 0, 1, 0, 0, 1, 0, 0, 0}, {0, 1, 0, 0, 0, 1, 1, 0, 1, 0},
            {0, 1, 1, 1, 0, 0, 0, 0, 1, 0}, {0, 0, 0, 1, 0, 1, 1, 0, 1, 0},
            {0, 1, 0, 0, 0, 0, 1, 0, 0, 0}, {0, 1, 1, 1, 1, 0, 1, 1, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    width = map[0].size();
    height = map.size();
}

bool Map::isWalkable(const int x, const int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height)
        return false;

    return map[y][x] == 0;
}

bool Map::isBombSite(int x, int y) const { return (x == 1 && y == 1); }


void Map::show() const {
    for (const auto& row: map) {
        for (const int cell: row) {
            std::cout << (cell == 0 ? "." : "#");
        }
        std::cout << std::endl;
    }
}
