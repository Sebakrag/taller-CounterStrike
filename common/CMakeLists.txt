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
        game_info/item_info.cpp

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
        game_info/item_info.h

        utils/Vec2D.h
        utils/EntityType.h

        dtos/EntitySnapshot.h
        dtos/MapInfo.h
        dtos/MatchInfo.h
        dtos/WindowConfig.h
    )
