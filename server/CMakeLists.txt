target_sources(taller_server
    PRIVATE
    # .cpp files
        main.cpp
        src/server.cpp
        src/client_handler.cpp
        src/player.cpp
        src/map.cpp
        src/weapon/weapon_knife.cpp
        src/weapon/fire_weapon.cpp
        src/weapon/weapon_ak47.cpp
        src/match.cpp
    PUBLIC
    # .h files
        include/server.h
        include/client_handler.h
        include/player.h
        include/map.h
        include/weapon/weapon_knife.h
        include/weapon/weapon.h
        include/weapon/fire_weapon.h
        include/weapon/weapon_ak47.h
        include/match.h
        include/enums/player_type.h
        include/enums/player_state.h
        include/enums/weapon_type.h
        include/enums/match_phase.h
    )
