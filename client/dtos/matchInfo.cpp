#include "matchInfo.h"

match_info_t::match_info_t(const std::string& name, const window_config_t& win_config,
                           const std::string& map_scene):
        name(name), win_config(win_config), map_scene(map_scene) {}

const std::string match_info_t::get_name() const { return name; }

const window_config_t match_info_t::get_window_config() const { return win_config; }

const std::string match_info_t::get_map_scene() const { return map_scene; }
