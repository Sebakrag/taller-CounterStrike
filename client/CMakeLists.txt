target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    src/client_protocol.cpp
    src/client_sender.cpp
    src/client_receiver.cpp
    src/client.cpp

    PUBLIC
    # .h files
    include/client_protocol.h
    include/client_sender.h
    include/client_receiver.h
    include/client.h
    )
