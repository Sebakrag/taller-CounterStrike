// tests/test_protocol_game.cpp

#include <chrono>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include "../client/include/client_protocol.h"
#include "../common/constants_protocol.h"
#include "../common/socket.h"
#include "../server/include/server_protocol.h"

static constexpr uint16_t TEST_PORT = 12348;
static const char TEST_IP[] = "127.0.0.1";

// —————————————————————————————————————————————————
// Servidores simulados para GameAction
// —————————————————————————————————————————————————

void server_thread_buy_weapon(Weapon w) {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    // Login
    proto.recvUsername();
    proto.sendConfirmation(true);

    // Recibir BuyWeapon
    GameAction ga = proto.recvGameAction();
    EXPECT_EQ(ga.type, BuyWeapon);
    EXPECT_EQ(ga.weapon, w);
    listener.close();
}

void server_thread_buy_ammo(AmmoType ammoType, int count) {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    proto.recvUsername();
    proto.sendConfirmation(true);

    GameAction ga = proto.recvGameAction();
    EXPECT_EQ(ga.type, BuyAmmo);
    EXPECT_EQ(ga.ammoType, ammoType);
    EXPECT_EQ(ga.count_ammo, count);
    listener.close();
}

void server_thread_attack_walk(GameActionType type, float x, float y) {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    proto.recvUsername();
    proto.sendConfirmation(true);

    GameAction ga = proto.recvGameAction();
    EXPECT_EQ(ga.type, type);
    EXPECT_FLOAT_EQ(ga.direction.getX(), x);
    EXPECT_FLOAT_EQ(ga.direction.getY(), y);
    listener.close();
}

void server_thread_change_weapon(TypeWeapon typeWeapon) {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    proto.recvUsername();
    proto.sendConfirmation(true);

    GameAction ga = proto.recvGameAction();
    EXPECT_EQ(ga.type, ChangeWeapon);
    EXPECT_EQ(ga.typeWeapon, typeWeapon);
    listener.close();
}

void server_thread_pickup() {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    proto.recvUsername();
    proto.sendConfirmation(true);

    GameAction ga = proto.recvGameAction();
    EXPECT_EQ(ga.type, PickUp);
    listener.close();
}

void server_thread_rotate(float angle) {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    proto.recvUsername();
    proto.sendConfirmation(true);

    GameAction ga = proto.recvGameAction();
    EXPECT_EQ(ga.type, Rotate);
    EXPECT_FLOAT_EQ(ga.angle, angle);
    listener.close();
}

// —————————————————————————————————————————————————
// TESTS
// —————————————————————————————————————————————————

TEST(ProtocoloGameAction, ComprarArma) {
    std::thread srv([] { server_thread_buy_weapon(Weapon::Awp); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "Tester");
    client.recvConfirmation();

    client.sendGameAction(GameAction(BuyWeapon, Weapon::Awp));

    srv.join();
}

TEST(ProtocoloGameAction, ComprarMunicion) {
    std::thread srv([] { server_thread_buy_ammo(AmmoType::Primary, 42); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "Tester");
    client.recvConfirmation();

    client.sendGameAction(GameAction(BuyAmmo, AmmoType::Primary, 42));

    srv.join();
}

TEST(ProtocoloGameAction, Ataque) {
    float x = 0.3f, y = -0.7f;
    std::thread srv([&] { server_thread_attack_walk(Attack, x, y); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "Tester");
    client.recvConfirmation();

    client.sendGameAction(GameAction(Attack, Vec2D(x, y)));

    srv.join();
}

TEST(ProtocoloGameAction, Caminar) {
    float x = 1.0f, y = 0.0f;
    std::thread srv([&] { server_thread_attack_walk(Walk, x, y); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "Tester");
    client.recvConfirmation();

    client.sendGameAction(GameAction(Walk, Vec2D(x, y)));

    srv.join();
}

TEST(ProtocoloGameAction, CambiarArma) {
    std::thread srv([] { server_thread_change_weapon(TypeWeapon::Secondary); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "Tester");
    client.recvConfirmation();

    client.sendGameAction(GameAction(ChangeWeapon, TypeWeapon::Secondary));

    srv.join();
}

TEST(ProtocoloGameAction, RecogerItem) {
    std::thread srv([] { server_thread_pickup(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "Tester");
    client.recvConfirmation();

    client.sendGameAction(GameAction(PickUp));

    srv.join();
}

TEST(ProtocoloGameAction, Rotar) {
    float angle = 1.2345f;
    std::thread srv([&] { server_thread_rotate(angle); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "Tester");
    client.recvConfirmation();

    client.sendGameAction(GameAction(Rotate, angle));

    srv.join();
}
