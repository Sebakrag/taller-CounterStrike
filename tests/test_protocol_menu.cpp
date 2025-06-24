#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include <gtest/gtest.h>

#include "../client/include/client_protocol.h"
#include "../common/constants_protocol.h"
#include "../common/socket.h"
#include "../server/include/server_protocol.h"

static constexpr uint16_t TEST_PORT = 12346;
static const char TEST_IP[] = "127.0.0.1";


// funciones para simular un servidor
//-----------------------------------

// Helper: servidor que acepta login y CreateMatch, responde según `ok`
void server_thread_create(bool ok) {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    // Login
    std::string user = proto.recvUsername();
    EXPECT_EQ(user, "JugadorX");
    proto.sendConfirmation(true);

    // CreateMatch
    auto action = proto.recvMenuAction();
    EXPECT_EQ(action.type, MenuActionType::Create);
    EXPECT_EQ(action.name_match, "SalaX");
    EXPECT_EQ(action.scenario_name, "A");
    proto.sendConfirmation(ok);

    listener.close();
}

// Helper: servidor que acepta login y JoinMatch, responde según `ok`
void server_thread_join(bool ok) {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    // Login
    std::string user = proto.recvUsername();
    EXPECT_EQ(user, "JugadorX");
    proto.sendConfirmation(true);

    // JoinMatch
    auto action = proto.recvMenuAction();
    EXPECT_EQ(action.type, MenuActionType::Join);
    EXPECT_EQ(action.name_match, "SalaX");
    proto.sendConfirmation(ok);

    listener.close();
}

// Helper: servidor que acepta login y ListMatches, envía la lista `matches`
void server_thread_list(const std::vector<std::string>& matches) {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    // Login
    std::string user = proto.recvUsername();
    EXPECT_EQ(user, "JugadorX");
    proto.sendConfirmation(true);

    // ListMatches
    auto action = proto.recvMenuAction();
    EXPECT_EQ(action.type, MenuActionType::List);

    std::list<std::string> lm(matches.begin(), matches.end());
    proto.sendListOfMatchs(lm);

    listener.close();
}

//
// TESTS
//

TEST(ProtocoloCrearPartida, Exito) {
    std::thread srv([] { server_thread_create(true); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "JugadorX");
    client.recvConfirmation();
    // El login ya se confirma en el constructor

    client.sendMenuAction(MenuAction(MenuActionType::Create, "SalaX", "A"));
    bool created = client.recvConfirmation();
    EXPECT_TRUE(created);

    srv.join();
}

TEST(ProtocoloCrearPartida, Falla) {
    std::thread srv([] { server_thread_create(false); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "JugadorX");
    client.recvConfirmation();

    client.sendMenuAction(MenuAction(MenuActionType::Create, "SalaX", "A"));
    bool created = client.recvConfirmation();
    EXPECT_FALSE(created);

    srv.join();
}

TEST(ProtocoloListarPartidas, DevuelveListaCorrecta) {
    std::vector<std::string> expected = {"SalaA", "SalaB", "SalaC"};
    std::thread srv([&] { server_thread_list(expected); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "JugadorX");
    client.recvConfirmation();

    client.sendMenuAction(MenuAction(MenuActionType::List));
    auto list = client.recvListMatchs();
    EXPECT_EQ(list, expected);

    srv.join();
}

TEST(ProtocoloUnirsePartida, Exito) {
    std::thread srv([] { server_thread_join(true); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "JugadorX");
    client.recvConfirmation();

    client.sendMenuAction(MenuAction(MenuActionType::Join, "SalaX"));
    bool joined = client.recvConfirmation();
    EXPECT_TRUE(joined);

    srv.join();
}

TEST(ProtocoloUnirsePartida, Falla) {
    std::thread srv([] { server_thread_join(false); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "JugadorX");
    client.recvConfirmation();

    client.sendMenuAction(MenuAction(MenuActionType::Join, "SalaX"));
    bool joined = client.recvConfirmation();
    EXPECT_FALSE(joined);

    srv.join();
}
