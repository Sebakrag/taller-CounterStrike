#ifndef MATCHINFO_H
#define MATCHINFO_H

#include <string>

#include "EntitySnapshot.h"
#include "MapInfo.h"
#include "WindowConfig.h"

struct MatchInfo {
    const std::string name;
    const EntitySnapshot first_snap;  // Representa el snapshot inicial del local_player
    const WindowConfig win_config;
    const MapInfo map_info;
    // const std::list<players_info_t> players;  // pensar si es la mejor forma.

    MatchInfo(const std::string& name, const EntitySnapshot& first_snap,
              const WindowConfig& win_config, const MapInfo& map_info):
            name(name), first_snap(first_snap), win_config(win_config), map_info(map_info) {}
};

#endif  // MATCHINFO_H
