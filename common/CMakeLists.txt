target_sources(taller_common
    PRIVATE
    # .cpp files
        socket.cpp
        resolver.cpp
        liberror.cpp
        resolvererror.cpp

        game_info.cpp
        protocol.cpp
        tile.cpp
        tile_map.cpp
    PUBLIC
    # .h files
        socket.h
        resolver.h
        liberror.h
        resolvererror.h
        thread.h

        constants_protocol.h
        game_info.h
        protocol.h
        types.h
        tile.h
        tile_map.h
    )
