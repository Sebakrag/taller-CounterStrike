target_sources(taller_server
    PRIVATE
    # .cpp files
        acceptor.cpp
        client_handler.cpp
        game_manager.cpp
        match.cpp
        main.cpp
        server_protocol.cpp
        server.cpp

    PUBLIC
    # .h files
        acceptor.h
        client_handler.h
        game_manager.h
        match.h
        server_protocol.h
        server.h
    )
