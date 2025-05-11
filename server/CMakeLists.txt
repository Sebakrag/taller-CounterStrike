target_sources(taller_server
    PRIVATE
    # .cpp files
        main.cpp
        server.cpp
        client_handler.cpp
    PUBLIC
    # .h files
        server.h
        client_handler.h
    )
