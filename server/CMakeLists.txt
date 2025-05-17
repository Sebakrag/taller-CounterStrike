target_sources(taller_server
    PRIVATE
    # .cpp files
        main.cpp
        server.cpp
        client_handler.cpp
        player.cpp
        map.cpp
        weapon_knife.cpp
        weapon/fire_weapon.cpp
        weapon/weapon_ak47.cpp
    PUBLIC
    # .h files
        server.h
        client_handler.h
        player.h
        map.h
        weapon_knife.h
        weapon/weapon.h
        weapon/fire_weapon.h
        weapon/weapon_ak47.h
        enums/player_type.h
        enums/player_state.h
        enums/weapon_type.h
    )
