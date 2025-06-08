#ifndef MATCHINFO_H
#define MATCHINFO_H

#include <string>

#include "../tile_map.h"

#include "EntitySnapshot.h"
#include "WindowConfig.h"

struct MatchInfo {
    std::string name;  // nombre de la partida.
    WindowConfig win_config;
    TileMap tileMap;

    MatchInfo() {}

    explicit MatchInfo(const std::string& name, const WindowConfig& win_config,
                       const TileMap& tile_map):
            name(name), win_config(win_config), tileMap(tile_map) {}


    MatchInfo(const MatchInfo& other) = default;

    MatchInfo& operator=(const MatchInfo& other) = default;
};

#endif  // MATCHINFO_H
