target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
        src/app-state/AppStateController.cpp
        src/app-state/LoginAppState.cpp
        src/app-state/MainMenuAppState.cpp
        src/app-state/GameMatchAppState.cpp
        model/Game.cpp
        model/Map.cpp
        dtos/matchInfo.cpp
        dtos/windowConfig.cpp
    PUBLIC
    # .h files
        include/app-state/AppState.h
        include/app-state/AppStateCode.h
        include/app-state/AppStateController.h
        include/app-state/LoginAppState.h
        include/app-state/MainMenuAppState.h
        include/app-state/GameMatchAppState.h
        model/Game.h
        model/Map.h
        dtos/matchInfo.h
        dtos/windowConfig.h
    )
