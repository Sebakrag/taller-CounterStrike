target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
        # app-sates:
        src/app-state/AppStateController.cpp
        src/app-state/LoginAppState.cpp
        src/app-state/MainMenuAppState.cpp
        src/app-state/GameMatchAppState.cpp
        # model:
        src/model/Game.cpp
        src/model/Map.cpp
        src/model/Graphics.cpp
        src/model/World.cpp
        src/model/EC/EntityManager.cpp
        src/model/EC/components/PositionComponent.cpp
        src/model/utils/InputHandler.cpp
        src/model/utils/Vec2D.cpp
        # dtos:
        dtos/matchInfo.cpp
        dtos/windowConfig.cpp
        PUBLIC
    # .h files
        # app-sates:
        include/app-state/AppState.h
        include/app-state/AppStateCode.h
        include/app-state/AppStateController.h
        include/app-state/LoginAppState.h
        include/app-state/MainMenuAppState.h
        include/app-state/GameMatchAppState.h
        # model:
        include/model/Game.h
        include/model/Graphics.h
        include/model/Map.h
        include/model/World.h
        include/model/EC/Entity.h
        include/model/EC/EntityManager.h
        include/model/EC/ComponentManager.h
        include/model/EC/ComponentPool.h
        include/model/EC/components/InputComponent.h
        include/model/EC/components/PositionComponent.h
        include/model/utils/InputHandler.h
        include/model/utils/Vec2D.h
        # dtos:
        dtos/matchInfo.h
        dtos/windowConfig.h
    )
