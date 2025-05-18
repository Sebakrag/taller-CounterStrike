target_sources(taller_server
    PRIVATE
    # .cpp files
        main.cpp
        server.cpp
        client_handler.cpp
        player.cpp
        map.cpp
        weapon/weapon_knife.cpp
        weapon/fire_weapon.cpp
        weapon/weapon_ak47.cpp
        match.cpp
    PUBLIC
    # .h files
        server.h
        client_handler.h
        player.h
        map.h
        weapon/weapon_knife.h
        weapon/weapon.h
        weapon/fire_weapon.h
        weapon/weapon_ak47.h
        match.h
        enums/player_type.h
        enums/player_state.h
        enums/weapon_type.h
        enums/match_phase.h
    )
