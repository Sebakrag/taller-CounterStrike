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
    )
