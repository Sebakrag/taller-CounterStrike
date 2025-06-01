#ifndef MAP_H
#define MAP_H

#include <vector>

class Map {
private:
    std::vector<std::vector<int>> map;
    int width, height;

public:
    Map();
    bool isWalkable(const int x, const int y) const;
    bool isBombSite(int x, int y) const;
    void show() const;
};

#endif
