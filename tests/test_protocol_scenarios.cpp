// tests/test_protocol_scenarios.cpp

#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include "../client/include/client_protocol.h"
#include "../common/constants_protocol.h"
#include "../common/socket.h"
#include "../server/include/server_protocol.h"

static constexpr uint16_t TEST_PORT = 12351;
static const char TEST_IP[] = "127.0.0.1";

// -----------------------------
// Servidores simulados
// -----------------------------

// 1) Envío correcto de lista de escenarios
void servidor_listar_escenarios(const std::vector<std::string>& escenarios) {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    proto.recvUsername();
    proto.sendConfirmation(true);

    proto.sendListOfScenarios(escenarios);

    listener.close();
}

// 2) Envío de un byte de encabezado incorrecto
void servidor_encabezado_incorrecto() {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    proto.recvUsername();
    proto.sendConfirmation(true);

    // Enviar byte equivocado
    proto.sendConfirmation(false);

    listener.close();
}

// 3) Cerrar sin enviar nada tras login
void servidor_cierre_inmediato() {
    Socket listener(std::to_string(TEST_PORT).c_str());
    Socket peer = listener.accept();
    ServerProtocol proto(std::move(peer));

    proto.recvUsername();
    proto.sendConfirmation(true);

    // Cerrar sin más envíos
    listener.close();
}

// -----------------------------
// Tests
// -----------------------------

TEST(ProtocoloEscenarios, RetornaListaCorrecta) {
    std::vector<std::string> expected = {"Desert", "Aztec", "Training"};
    std::thread srv([&] { servidor_listar_escenarios(expected); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "Tester");
    client.recvConfirmation();

    auto list = client.recvListScenaries();
    EXPECT_EQ(list, expected);

    srv.join();
}

TEST(ProtocoloEscenarios, Excepcion_EncabezadoIncorrecto) {
    std::thread srv([] { servidor_encabezado_incorrecto(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "Tester");
    client.recvConfirmation();

    EXPECT_THROW(client.recvListScenaries(), std::runtime_error);

    srv.join();
}

TEST(ProtocoloEscenarios, Excepcion_SinDatos) {
    std::thread srv([] { servidor_cierre_inmediato(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ClientProtocol client(TEST_IP, std::to_string(TEST_PORT), "Tester");
    client.recvConfirmation();

    EXPECT_THROW(client.recvListScenaries(), std::runtime_error);

    srv.join();
}
