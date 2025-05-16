target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    connection_info.cpp
        app-state/AppStateController.cpp
        app-state/LoginAppState.cpp
        app-state/MainMenuAppState.cpp
        app-state/GameMatchAppState.cpp
        model/Game.cpp
        model/Map.cpp
        dtos/matchInfo.cpp
        dtos/windowConfig.cpp
    PUBLIC
    # .h files
    connection_info.h
        app-state/AppState.h
        app-state/AppStateCode.h
        app-state/AppStateController.h
        app-state/LoginAppState.h
        app-state/MainMenuAppState.h
        app-state/GameMatchAppState.h
        model/Game.h
        model/Map.h
        dtos/matchInfo.h
        dtos/windowConfig.h
    )
