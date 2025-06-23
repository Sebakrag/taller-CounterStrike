#include <chrono>
#include <string>
#include <thread>

#include <gtest/gtest.h>

#include "../client/include/client_protocol.h"
#include "../common/constants_protocol.h"
#include "../common/socket.h"
#include "../server/include/server_protocol.h"

static constexpr uint16_t TEST_PORT = 12345;
static const char TEST_IP[] = "127.0.0.1";

// -----------------------------
// Servidores simulados para Login
// -----------------------------

void server_thread_login(bool accept_login) {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    std::string user = proto.recvUsername();
    EXPECT_EQ(user, "Jugador1");
    proto.sendConfirmation(accept_login);

    listener.close();
}

// -----------------------------
// TESTS
// -----------------------------

TEST(ProtocoloLogin, Exito) {
    std::thread srv([] { server_thread_login(true); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "Jugador1");
    bool ok = client.recvConfirmation();
    EXPECT_TRUE(ok) << "❌ El servidor NO aceptó el login";

    srv.join();
}

TEST(ProtocoloLogin, Falla) {
    std::thread srv([] { server_thread_login(false); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "Jugador1");
    bool ok = client.recvConfirmation();
    EXPECT_FALSE(ok) << "❌ El servidor debería haber rechazado el login";

    srv.join();
}
