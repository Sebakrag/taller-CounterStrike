# Fuentes del cliente
target_sources(taller_client
    PRIVATE
        # Recursos Qt
        ${RCC_SOURCES}
        
        # Lógica principal
        main.cpp
        # app-sates:
        src/app-state/AppStateController.cpp
        src/app-state/LobbyAppState.cpp
        src/app-state/LoginAppState.cpp
        src/app-state/MainMenuAppState.cpp
        src/app-state/GameMatchAppState.cpp
        # ui:
        src/ui/JoinGameDialog.cpp
        src/ui/LobbyWindow.cpp
        src/ui/LoginWindow.cpp
        src/ui/MainMenuWindow.cpp
        src/ui/ScenarioSelectionDialog.cpp
        src/ui/NameInputDialog.cpp
        src/ui/StyledMessageBox.cpp
        src/ui/AudioManager.cpp
        # model:
        src/model/Camera.cpp
        src/model/FieldOfView.cpp
        src/model/Game.cpp
        src/model/Map.cpp
        src/model/Graphics.cpp
        src/model/HUD.cpp
        src/model/World.cpp
        src/model/EC/AudioSystem.cpp
        src/model/EC/ComponentUpdater.cpp
        src/model/EC/EntityFactory.cpp
        src/model/EC/EntityManager.cpp
        src/model/EC/RenderSystem.cpp
        src/model/EC/components/PlayerSpriteComponent.cpp
        src/model/EC/components/SoundComponent.cpp
        src/model/EC/components/TransformComponent.cpp
        src/model/EC/components/SpriteComponent.cpp
        src/model/EC/components/WeaponSpriteComponent.cpp
        src/model/utils/DynamicStencil.cpp
        src/model/utils/EventHandler.cpp
        src/model/utils/NumberRenderer.cpp
        src/model/utils/SoundLibrary.cpp
        src/model/utils/SpriteMetadaProvider.cpp
        src/model/utils/SymbolsRenderer.cpp
        src/model/utils/TextureManager.cpp
        # protocol:
        src/client_protocol.cpp
        src/client_sender.cpp
        src/client_receiver.cpp
        src/client.cpp
        
    PUBLIC
    # .h files
        client_constants.h
        # app-sates:
        include/app-state/AppState.h
        include/app-state/AppStateCode.h
        include/app-state/AppStateController.h
        include/app-state/LoginAppState.h
        include/app-state/LobbyAppState.h
        include/app-state/MainMenuAppState.h
        include/app-state/GameMatchAppState.h
        # ui:
        include/ui/JoinGameDialog.h
        include/ui/LobbyWindow.h
        include/ui/LoginWindow.h
        include/ui/MainMenuWindow.h
        include/ui/NameInputDialog.h
        include/ui/ScenarioSelectionDialog.h
        include/ui/StyledMessageBox.h
        include/ui/UIConstants.h
        include/ui/AudioManager.h
        # model:
        include/model/Camera.h
        include/model/FieldOfView.h
        include/model/Game.h
        include/model/Graphics.h
        include/model/HUD.h
        include/model/Map.h
        include/model/World.h
        include/model/EC/AudioSystem.h
        include/model/EC/Entity.h
        include/model/EC/EntityFactory.h
        include/model/EC/EntityManager.h
        include/model/EC/ComponentManager.h
        include/model/EC/ComponentPool.h
        include/model/EC/ComponentUpdater.h
        include/model/EC/RenderSystem.h
        include/model/EC/components/BulletSpriteComponent.h
        include/model/EC/components/EquippedWeaponComponent.h
        include/model/EC/components/PlayerSpriteComponent.h
        include/model/EC/components/SoundComponent.h
        include/model/EC/components/SpriteComponent.h
        include/model/EC/components/TransformComponent.h
        include/model/EC/components/WeaponSpriteComponent.h
        include/model/utils/DynamicStencil.h
        include/model/utils/EventHandler.h
        include/model/utils/NumberRenderer.h
        include/model/utils/SoundEvent.h
        include/model/utils/SoundLibrary.h
        include/model/utils/SpriteMetadataProvider.h
        include/model/utils/SymbolsRenderer.h
        include/model/utils/TextureManager.h
        # dtos:
        dtos/AimInfo.h
        dtos/FOVInfo.h
        # protocol:
        include/client_protocol.h
        include/client_sender.h
        include/client_receiver.h
        include/client.h
    )

# Directorios de inclusión
target_include_directories(taller_client
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
