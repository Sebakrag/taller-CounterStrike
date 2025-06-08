#ifndef LIST_SCENARIES_H_
#define LIST_SCENARIES_H_

#include <map>
#include <string>
#include <vector>

#include "../../common/tile_map.h"

struct Scenary {
    std::string name;
    TileMap tilemap;
};

class ListScenaries {
private:
    std::map<std::string, TileMap> scenaries;

public:
    explicit ListScenaries(std::vector<Tile>);
};

#endif  // LIST_SCENARIES_H_
