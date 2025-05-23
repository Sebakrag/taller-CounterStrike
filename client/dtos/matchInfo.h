#ifndef MATCHINFO_H
#define MATCHINFO_H

#include <cstdint>
#include <string>

#include "windowConfig.h"

struct match_info_t {
private:
    const std::string name;
    const uint32_t server_entity_id;  // Esto indica el id del jugador identificado por el server.
    const window_config_t win_config;
    const std::string map_scene;  // filename del mapa a utilizar?
    // const std::list<players_info_t> players;  // pensar si es la mejor forma.
public:
    match_info_t(const std::string& name, const uint32_t server_entity_id,
                 const window_config_t& win_config, const std::string& map_scene);

    const std::string get_name() const;
    uint32_t get_server_entity_id() const;
    const window_config_t get_window_config() const;
    const std::string get_map_scene() const;
};

#endif  // MATCHINFO_H
