target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    connection_info.cpp
        app-state/AppStateController.cpp
        app-state/LoginAppState.cpp
        app-state/MainMenuAppState.cpp
        app-state/GameMatchAppState.cpp
    PUBLIC
    # .h files
    connection_info.h
        app-state/AppState.h
        app-state/AppStateCode.h
        app-state/AppStateController.h
        app-state/LoginAppState.h
        app-state/MainMenuAppState.h
        app-state/GameMatchAppState.h
    )
