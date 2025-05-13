target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    connection_info.cpp
    AppStateController.cpp
    LoginAppState.cpp
    MainMenuAppState.cpp
    GameMatchAppState.cpp
    PUBLIC
    # .h files
    connection_info.h
    AppState.h
    AppStateCode.h
    AppStateController.h
    LoginAppState.h
    MainMenuAppState.h
    GameMatchAppState.h
    )
