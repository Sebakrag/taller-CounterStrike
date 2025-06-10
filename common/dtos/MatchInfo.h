#ifndef MATCHINFO_H
#define MATCHINFO_H

#include <string>

#include "common/tile_map.h"

#include "WindowConfig.h"

struct MatchInfo {
    std::string name;  // nombre de la partida.
    WindowConfig win_config;
    TileMap tileMap;

    MatchInfo() = default;

    explicit MatchInfo(const std::string& name, const WindowConfig& win_config,
                       const TileMap& tile_map):
            name(name), win_config(win_config), tileMap(tile_map) {}


    MatchInfo(const MatchInfo& other) = default;

    MatchInfo& operator=(const MatchInfo& other) = default;

    void print() const {
        std::cout << "MatchInfo {\n";
        std::cout << "  Name: " << name << "\n";
        std::cout << "  WindowConfig: { width: " << win_config.width
                  << ", height: " << win_config.height << ", flags: " << win_config.flags << " }\n";
        std::cout << "  TileMap: [size: " << tileMap.getColCount() << " x " << tileMap.getRowCount()
                  << "]\n";
        std::cout << "}" << std::endl;
    }
};

#endif  // MATCHINFO_H
