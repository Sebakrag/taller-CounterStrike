#ifndef MATCHINFO_H
#define MATCHINFO_H

#include <string>

#include "EntitySnapshot.h"
#include "MapInfo.h"
#include "windowConfig.h"

struct match_info_t {
public:
    const std::string name;
    const EntitySnapshot first_snap;  // Representa el snapshot inicial del local_player
    const window_config_t win_config;
    const MapInfo map_info;
    // const std::list<players_info_t> players;  // pensar si es la mejor forma.

    match_info_t(const std::string& name, const EntitySnapshot& first_snap,
                 const window_config_t& win_config, const MapInfo& map_info):
            name(name), first_snap(first_snap), win_config(win_config), map_info(map_info) {}
};

#endif  // MATCHINFO_H
