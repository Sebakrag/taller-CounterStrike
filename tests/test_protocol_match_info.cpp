// tests/test_protocol_matchinfo.cpp

#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include "../client/include/client_protocol.h"
#include "../common/constants_protocol.h"
#include "../common/dtos/MatchInfo.h"
#include "../common/game_info/local_player_info.h"
#include "../common/socket.h"
#include "../server/include/server_protocol.h"

static constexpr uint16_t TEST_PORT = 12350;
static const char TEST_IP[] = "127.0.0.1";

void server_thread_send_matchinfo(const MatchInfo& info) {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    proto.recvUsername();  // Login
    proto.sendConfirmation(true);

    proto.sendMatchInfo(info);

    listener.close();
}

TEST(ProtocoloMatchInfo, ClienteRecibeMatchInfoCorrectamente) {
    // 1) Configuración de ventana
    WindowConfig win_conf{800, 600, 0};

    // 2) FOVConfig
    FOVConfig fov_conf;
    fov_conf.isActive = true;
    fov_conf.screenWidth = 800;
    fov_conf.screenHeight = 600;
    fov_conf.circleRadius = 100.0f;
    fov_conf.fovAngle = 90.0f;
    fov_conf.visibilityDistance = 300.0f;
    fov_conf.transparency = 0.5f;

    // 3) Mapa 2×2 con IDs de tiles
    std::vector<std::vector<int>> tile_ids = {{0, 1}, {2, 1}};
    TileMap tile_map(TypeTileMap::Aztec, tile_ids);

    // 4) LocalPlayerInfo
    LocalPlayerInfo player(1,                              // server_entt_id
                           Team::CounterTerrorist,         // team
                           PlayerSkin::CounterTerrorist1,  // skin
                           PlayerState::Idle,              // state
                           Vec2D(10.0f, 5.0f),             // position
                           1.57f,                          // angle_direction
                           TypeWeapon::Secondary,          // weapon_type
                           Weapon::Glock,                  // weapon
                           100,                            // health
                           2000,                           // money
                           30,                             // ammo
                           24);

    // 5) Montar MatchInfo
    MatchInfo info("SalaTest", win_conf, fov_conf, tile_map, 2, player);

    // 6) Servidor en hilo
    std::thread srv([&] { server_thread_send_matchinfo(info); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    // 7) Cliente recibe
    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "Jugador1");
    client.recvConfirmation();

    MatchInfo received = client.recvMatchInfo();

    // — Verificaciones —

    // Nombre y número de jugadores
    EXPECT_EQ(received.name, "SalaTest");
    EXPECT_EQ(received.numPlayers, 2);

    // WindowConfig
    EXPECT_EQ(received.win_config.width, 800);
    EXPECT_EQ(received.win_config.height, 600);
    EXPECT_EQ(received.win_config.flags, 0);

    // FOVConfig
    EXPECT_TRUE(received.fovConfig.isActive);
    EXPECT_EQ(received.fovConfig.screenWidth, 800);
    EXPECT_EQ(received.fovConfig.screenHeight, 600);
    EXPECT_FLOAT_EQ(received.fovConfig.circleRadius, 100.0f);
    EXPECT_FLOAT_EQ(received.fovConfig.fovAngle, 90.0f);
    EXPECT_FLOAT_EQ(received.fovConfig.visibilityDistance, 300.0f);
    EXPECT_FLOAT_EQ(received.fovConfig.transparency, 0.5f);

    // TileMap tamaño y contenido
    EXPECT_EQ(received.tileMap.getColCount(), 2);
    EXPECT_EQ(received.tileMap.getRowCount(), 2);
    EXPECT_EQ(received.tileMap.getIdTile(0, 0), 0);
    EXPECT_EQ(received.tileMap.getIdTile(0, 1), 1);
    EXPECT_EQ(received.tileMap.getIdTile(1, 0), 2);
    EXPECT_EQ(received.tileMap.getIdTile(1, 1), 1);

    // LocalPlayerInfo
    const auto& lp = received.localPlayerInfo;
    EXPECT_EQ(lp.server_entt_id, 1);
    EXPECT_EQ(lp.team, Team::CounterTerrorist);
    EXPECT_EQ(lp.skin, PlayerSkin::CounterTerrorist1);
    EXPECT_EQ(lp.state, PlayerState::Idle);
    EXPECT_FLOAT_EQ(lp.position.getX(), 10.0f);
    EXPECT_FLOAT_EQ(lp.position.getY(), 5.0f);
    EXPECT_FLOAT_EQ(lp.angle_direction, 1.57f);
    EXPECT_EQ(lp.weapon_type, TypeWeapon::Secondary);
    EXPECT_EQ(lp.weapon, Weapon::Glock);
    EXPECT_EQ(lp.health, 100);
    EXPECT_EQ(lp.money, 2000);
    EXPECT_EQ(lp.ammo_weapon, 30);
    EXPECT_EQ(lp.equipped_weapon_id, 24);

    srv.join();
}
