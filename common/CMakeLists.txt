target_sources(taller_common
    PRIVATE
    # .cpp files
        socket.cpp
        resolver.cpp
        liberror.cpp
        resolvererror.cpp

        game_info.cpp
        protocol.cpp
    PUBLIC
    # .h files
        socket.h
        resolver.h
        liberror.h
        resolvererror.h
        threads.h

        constants_protocol.h
        game_info.h
        protocol.h
        types.h
    )
