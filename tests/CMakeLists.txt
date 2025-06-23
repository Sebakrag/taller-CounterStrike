target_sources(taller_tests
  PRIVATE
    test_protocol_login.cpp
    test_protocol_menu.cpp
    test_protocol_lobby.cpp
    test_protocol_game.cpp
    test_protocol_match_info.cpp
    test_protocol_scenarios.cpp
    test_protocol_game_info.cpp
    
    ../server/src/server_protocol.cpp
    
    ../client/src/client_protocol.cpp
    ../client/src/client.cpp               # ← lo vas a usar
    ../client/src/client_sender.cpp
    ../client/src/client_receiver.cpp

    ../common/socket.cpp
    ../common/protocol.cpp
    ../common/tile_map.cpp
)
target_include_directories(taller_tests
  PRIVATE
    ${CMAKE_SOURCE_DIR}/server/include
    ${CMAKE_SOURCE_DIR}/client/include
    ${CMAKE_SOURCE_DIR}/common          # tilemap, socket, etc.
)
