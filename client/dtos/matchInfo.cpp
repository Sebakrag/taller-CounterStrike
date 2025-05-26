#include "matchInfo.h"

match_info_t::match_info_t(const std::string& name, const EntitySnapshot& first_snap,
                           const window_config_t& win_config, const MapInfo& map_info):
        name(name), first_snap(first_snap), win_config(win_config), map_info(map_info) {}
