target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    src/client_protocol.cpp
    src/client.cpp

    PUBLIC
    # .h files
    include/client_protocol.h
    include/client.h
    )
