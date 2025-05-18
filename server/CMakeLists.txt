target_sources(taller_server
    PRIVATE
    # .cpp files
        src/acceptor.cpp
        src/client_handler.cpp
        src/game_manager.cpp
        src/match.cpp
        main.cpp
        src/server_protocol.cpp
        src/server.cpp

    PUBLIC
    # .h files
        include/acceptor.h
        include/client_handler.h
        include/game_manager.h
        include/match.h
        include/server_protocol.h
        include/server.h
    )
