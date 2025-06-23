#include <chrono>
#include <string>
#include <thread>

#include <gtest/gtest.h>

#include "../client/include/client_protocol.h"
#include "../common/constants_protocol.h"
#include "../common/socket.h"
#include "../server/include/server_protocol.h"

static constexpr uint16_t TEST_PORT = 12347;
static const char TEST_IP[] = "127.0.0.1";

// -----------------------------
// Servidor simulado para Lobby
// -----------------------------

void server_thread_list_players() {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    std::string user = proto.recvUsername();
    EXPECT_EQ(user, "JugadorX");
    proto.sendConfirmation(true);

    LobbyAction action = proto.recvLobbyAction();
    EXPECT_EQ(action, LobbyAction::ListPlayers);

    MatchRoomInfo info;
    info.matchStarted = false;
    info.players = {PlayerInfoLobby("JugadorX", Team::Terrorist, true),
                    PlayerInfoLobby("JugadorY", Team::CounterTerrorist, false)};
    proto.sendMatchRoomInfo(info);

    listener.close();
}
// 2) Lista con jugadores y matchStarted = true
void server_thread_list_started() {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    proto.recvUsername();
    proto.sendConfirmation(true);

    proto.recvLobbyAction();  // ListPlayers
    MatchRoomInfo info;
    info.matchStarted = true;
    info.players = {PlayerInfoLobby("Alice", Team::Terrorist, true),
                    PlayerInfoLobby("Bob", Team::CounterTerrorist, false)};
    proto.sendMatchRoomInfo(info);

    proto.shutDown(2);
    listener.close();
}

void server_thread_quit_match(bool allow_exit) {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    std::string user = proto.recvUsername();
    EXPECT_EQ(user, "JugadorX");
    proto.sendConfirmation(true);

    LobbyAction action = proto.recvLobbyAction();
    EXPECT_EQ(action, LobbyAction::QuitMatch);
    proto.sendConfirmation(allow_exit);

    listener.close();
}

void server_thread_start_match(bool allowed) {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    std::string user = proto.recvUsername();
    EXPECT_EQ(user, "JugadorX");
    proto.sendConfirmation(true);

    LobbyAction action = proto.recvLobbyAction();
    EXPECT_EQ(action, LobbyAction::StartMatch);
    proto.sendConfirmation(allowed);

    listener.close();
}

// -----------------------------
// TESTS
// -----------------------------

TEST(ProtocoloLobby, ListarJugadores) {
    std::thread srv([] { server_thread_list_players(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "JugadorX");
    client.recvConfirmation();

    client.sendLobbyAction(LobbyAction::ListPlayers);
    MatchRoomInfo info = client.recvUpdateMatchRoom();

    ASSERT_FALSE(info.matchStarted);
    ASSERT_EQ(info.players.size(), 2);
    EXPECT_EQ(info.players[0].username, "JugadorX");
    EXPECT_EQ(info.players[0].team, Team::Terrorist);
    EXPECT_TRUE(info.players[0].is_player_host);
    EXPECT_EQ(info.players[1].username, "JugadorY");
    EXPECT_EQ(info.players[1].team, Team::CounterTerrorist);
    EXPECT_FALSE(info.players[1].is_player_host);

    srv.join();
}
TEST(ProtocoloLobby, MatchYaIniciado) {
    std::thread srv([] { server_thread_list_started(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "Tester");
    client.recvConfirmation();

    client.sendLobbyAction(LobbyAction::ListPlayers);
    MatchRoomInfo info = client.recvUpdateMatchRoom();

    EXPECT_TRUE(info.matchStarted);
    ASSERT_EQ(info.players.size(), 2u);
    EXPECT_EQ(info.players[0].username, "Alice");
    EXPECT_EQ(info.players[0].team, Team::Terrorist);
    EXPECT_EQ(info.players[1].username, "Bob");
    EXPECT_EQ(info.players[1].team, Team::CounterTerrorist);

    srv.join();
}
TEST(ProtocoloLobby, AbandonarPartidaExito) {
    std::thread srv([] { server_thread_quit_match(true); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "JugadorX");
    client.recvConfirmation();

    client.sendLobbyAction(LobbyAction::QuitMatch);
    bool result = client.recvConfirmation();
    EXPECT_TRUE(result);

    srv.join();
}

TEST(ProtocoloLobby, AbandonarPartidaFalla) {
    std::thread srv([] { server_thread_quit_match(false); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "JugadorX");
    client.recvConfirmation();

    client.sendLobbyAction(LobbyAction::QuitMatch);
    bool result = client.recvConfirmation();
    EXPECT_FALSE(result);

    srv.join();
}

TEST(ProtocoloLobby, IniciarPartidaExito) {
    std::thread srv([] { server_thread_start_match(true); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "JugadorX");
    client.recvConfirmation();

    client.sendLobbyAction(LobbyAction::StartMatch);
    bool started = client.recvConfirmation();
    EXPECT_TRUE(started);

    srv.join();
}

TEST(ProtocoloLobby, IniciarPartidaFalla) {
    std::thread srv([] { server_thread_start_match(false); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "JugadorX");
    client.recvConfirmation();

    client.sendLobbyAction(LobbyAction::StartMatch);
    bool started = client.recvConfirmation();
    EXPECT_FALSE(started);

    srv.join();
}
