target_sources(taller_common
    PRIVATE
    # .cpp files
        socket.cpp
        resolver.cpp
        liberror.cpp
        resolvererror.cpp

        protocol.cpp
        tile.cpp
        tile_map.cpp

        game_info/player_info.cpp
        game_info/game_info.cpp
        game_info/bullet_info.cpp
        game_info/projectile_info.cpp
        game_info/bomb_info.cpp
        game_info/shop_info.cpp
        game_info/weapon_info.cpp
        game_info/local_player_info.cpp
        utils/AngleUtils.cpp
        utils/Vec2D.cpp
        dtos/EntitySnapshot.cpp

    PUBLIC
    # .h files
        socket.h
        resolver.h
        liberror.h
        resolvererror.h
        thread.h

        constants_protocol.h
        protocol.h
        types.h
        tile.h
        tile_map.h

        game_info/player_info.h
        game_info/game_info.h
        game_info/bullet_info.h
        game_info/projectile_info.h
        game_info/bomb_info.h
        game_info/shop_info.h
        game_info/weapon_info.h
        game_info/local_player_info.h

        utils/AngleUtils.h
        utils/Vec2D.h
        utils/EntityType.h

        dtos/AmmoInfo.h
        dtos/EntitySnapshot.h
        dtos/MatchInfo.h
        dtos/WindowConfig.h
        dtos/FovConfig.h
    )
