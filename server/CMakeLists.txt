target_sources(taller_server
    PRIVATE
    # .cpp files
        main.cpp
        src/acceptor.cpp
        src/client_handler.cpp
        src/game_manager.cpp
        src/gameloop.cpp
        src/match_room.cpp
        src/match.cpp
        src/receiver.cpp
        src/server_protocol.cpp
        src/server.cpp

    PUBLIC
    # .h files
        include/acceptor.h
        include/client_handler.h
        include/game_manager.h
        include/gameloop.h
        include/match_room.h
        include/match.h
        include/receiver.h
        include/server_protocol.h
        include/server.h
        include/types.h
    )
