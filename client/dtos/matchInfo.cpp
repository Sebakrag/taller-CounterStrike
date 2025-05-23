#include "matchInfo.h"

match_info_t::match_info_t(const std::string& name, const uint32_t server_entity_id,
                           const window_config_t& win_config, const std::string& map_scene):
        name(name),
        server_entity_id(server_entity_id),
        win_config(win_config),
        map_scene(map_scene) {}

const std::string match_info_t::get_name() const { return name; }

uint32_t match_info_t::get_server_entity_id() const { return server_entity_id; }

const window_config_t match_info_t::get_window_config() const { return win_config; }

const std::string match_info_t::get_map_scene() const { return map_scene; }
