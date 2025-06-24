// tests/test_protocol_gameinfo.cpp

#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include "../client/include/client_protocol.h"
#include "../common/constants_protocol.h"
#include "../common/game_info/bomb_info.h"
#include "../common/game_info/bullet_info.h"
#include "../common/game_info/game_info.h"
#include "../common/game_info/local_player_info.h"
#include "../common/game_info/player_info.h"
#include "../common/game_info/weapon_info.h"
#include "../common/socket.h"
#include "../common/types.h"
#include "../server/include/server_protocol.h"

static constexpr uint16_t TEST_PORT = 12353;
static const char TEST_IP[] = "127.0.0.1";

// Simula un servidor que envía un GameInfo completo
void server_thread_send_gameinfo(const GameInfo& info) {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    // Login
    proto.recvUsername();
    proto.sendConfirmation(true);

    // Envío del GameInfo
    proto.sendGameInfo(info);

    proto.shutDown(2);
    listener.close();
}

TEST(ProtocoloGameInfo, ClienteRecibeGameInfoCorrectamente) {
    // 1) Construir datos de prueba
    GamePhase phase = GamePhase::Preparation;

    BombInfo bomb(99, BombState::Planted, 5.5f, -3.3f);

    float timeLeft = 45.0f;

    LocalPlayerInfo local(1, Team::CounterTerrorist, PlayerSkin::CounterTerrorist2,
                          PlayerState::Walking, Vec2D(1.0f, 2.0f), 0.78f, TypeWeapon::Primary,
                          Weapon::Ak47, 75, 1200, 15, 42);

    std::vector<PlayerInfo> others = {
            PlayerInfo(2, "Alice", Team::Terrorist, PlayerSkin::Terrorist1, PlayerState::Attacking,
                       Vec2D(-1.0f, 0.5f), 1.2f, TypeWeapon::Secondary, Weapon::Glock, 60),
            PlayerInfo(3, "Bob", Team::CounterTerrorist, PlayerSkin::CounterTerrorist1,
                       PlayerState::Idle, Vec2D(0.0f, -1.0f), -0.5f, TypeWeapon::Knife,
                       Weapon::None, 100)};

    std::vector<BulletInfo> bullets = {
            BulletInfo(10, Weapon::Ak47, 2.0f, 3.0f, Vec2D(0.0f, 1.0f)),
            BulletInfo(11, Weapon::Glock, -2.5f, 4.4f, Vec2D(1.0f, 0.0f), false)};

    std::vector<WeaponInfo> weapons = {
            WeaponInfo(20, Weapon::M3, WeaponState::DROPPED, 8, 7.7f, 8.8f),
            WeaponInfo(21, Weapon::Awp, WeaponState::EQUIPPED, 5, -9.9f, 0.1f)};

    // TODO: sacar el shop cuando se actualice el constructor de gameInfo.
    GameInfo info(phase, bomb, timeLeft, local, others, bullets, weapons, StatsInfo());

    // 2) Arrancar servidor
    std::thread srv([&] { server_thread_send_gameinfo(info); });
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // 3) Cliente recibe
    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "Tester");
    client.recvConfirmation();

    GameInfo received = client.recvGameInfo();

    // 4) Verificaciones

    EXPECT_EQ(received.gamePhase, phase);
    EXPECT_EQ(received.bomb.server_entt_id, bomb.server_entt_id);
    EXPECT_EQ(received.bomb.state, bomb.state);
    EXPECT_FLOAT_EQ(received.bomb.pos_x, bomb.pos_x);
    EXPECT_FLOAT_EQ(received.bomb.pos_y, bomb.pos_y);

    EXPECT_FLOAT_EQ(received.timeLeft, timeLeft);

    // LocalPlayerInfo
    {
        const auto& lp = received.localPlayer;
        EXPECT_EQ(lp.server_entt_id, local.server_entt_id);
        EXPECT_EQ(lp.team, local.team);
        EXPECT_EQ(lp.skin, local.skin);
        EXPECT_EQ(lp.state, local.state);
        EXPECT_FLOAT_EQ(lp.position.getX(), local.position.getX());
        EXPECT_FLOAT_EQ(lp.position.getY(), local.position.getY());
        EXPECT_FLOAT_EQ(lp.angle_direction, local.angle_direction);
        EXPECT_EQ(lp.weapon_type, local.weapon_type);
        EXPECT_EQ(lp.weapon, local.weapon);
        EXPECT_EQ(lp.health, local.health);
        EXPECT_EQ(lp.money, local.money);
        EXPECT_EQ(lp.ammo_weapon, local.ammo_weapon);
        EXPECT_EQ(lp.equipped_weapon_id, local.equipped_weapon_id);
    }

    // Otros players
    ASSERT_EQ(received.otherPlayers.size(), others.size());
    for (size_t i = 0; i < others.size(); ++i) {
        const auto& exp = others[i];
        const auto& got = received.otherPlayers[i];
        EXPECT_EQ(got.server_entt_id, exp.server_entt_id);
        EXPECT_EQ(got.username, exp.username);
        EXPECT_EQ(got.team, exp.team);
        EXPECT_EQ(got.skin, exp.skin);
        EXPECT_EQ(got.state, exp.state);
        EXPECT_FLOAT_EQ(got.position.getX(), exp.position.getX());
        EXPECT_FLOAT_EQ(got.position.getY(), exp.position.getY());
        EXPECT_FLOAT_EQ(got.angle_direction, exp.angle_direction);
        EXPECT_EQ(got.weapon_type, exp.weapon_type);
        EXPECT_EQ(got.weapon, exp.weapon);
        EXPECT_EQ(got.equipped_weapon_id, exp.equipped_weapon_id);
    }

    // Bullets
    ASSERT_EQ(received.bullets.size(), bullets.size());
    for (size_t i = 0; i < bullets.size(); ++i) {
        const auto& exp = bullets[i];
        const auto& got = received.bullets[i];
        EXPECT_EQ(got.id, exp.id);
        EXPECT_EQ(got.weapon, exp.weapon);
        EXPECT_FLOAT_EQ(got.pos_x, exp.pos_x);
        EXPECT_FLOAT_EQ(got.pos_y, exp.pos_y);
        EXPECT_FLOAT_EQ(got.direction.getX(), exp.direction.getX());
        EXPECT_FLOAT_EQ(got.direction.getY(), exp.direction.getY());
        EXPECT_EQ(got.active, exp.active);
    }

    // Weapons
    ASSERT_EQ(received.weapons.size(), weapons.size());
    for (size_t i = 0; i < weapons.size(); ++i) {
        const auto& exp = weapons[i];
        const auto& got = received.weapons[i];
        EXPECT_EQ(got.server_entt_id, exp.server_entt_id);
        EXPECT_EQ(got.weapon, exp.weapon);
        EXPECT_EQ(got.state, exp.state);
        EXPECT_EQ(got.ammo, exp.ammo);
        EXPECT_FLOAT_EQ(got.pos_x, exp.pos_x);
        EXPECT_FLOAT_EQ(got.pos_y, exp.pos_y);
    }

    srv.join();
}
