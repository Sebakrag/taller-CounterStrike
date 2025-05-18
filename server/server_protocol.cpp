#include "server_protocol.h"

#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../common/constants_protocol.h"

ServerProtocol::ServerProtocol(Socket&& socketClient): Protocol_(std::move(socketClient)) {}

void ServerProtocol::sendConfirmation(bool ok) {
    uint8_t byte;
    if (ok)
        byte = BYTE_OK;
    else
        byte = BYTE_FAIL;

    socket.sendall(&byte, sizeof(uint8_t));
}

void ServerProtocol::sendInitMsg() {
    // std::vector<uint8_t> buffer;

    // Implementación de sendInitMsg
}

void ServerProtocol::sendMessage(TypeMessage typeMessage, const std::string& msg) {
    std::vector<uint8_t> buffer;
    if (typeMessage == ListMatchs)
        buffer.push_back(BYTE_MATCH_LIST);
    else  // if (typeMessage == ListPlayers)
        buffer.push_back(BYTE_PLAYERS_LIST);

    insertBigEndian16(msg.length(), buffer);
    insertStringBytes(msg, buffer);

    socket.sendall(buffer.data(), sizeof(uint8_t) * buffer.size());
}

void ServerProtocol::sendListPlayers(std::vector<PlayerInfoLobby> playersInMatch) {
    std::vector<uint8_t> buffer;
    buffer.push_back(BYTE_PLAYERS_LIST);
    int size = playersInMatch.size();
    insertBigEndian16(size, buffer);

    for (int i = 0; i < size; i++) {
        insertBigEndian16(playersInMatch[i].username.length(), buffer);
        insertStringBytes(playersInMatch[i].username, buffer);
        buffer.push_back(encodeTeam(playersInMatch[i].team));
    }
    socket.sendall(buffer.data(), sizeof(uint8_t) * buffer.size());
}

void ServerProtocol::sendGameInfo(const GameInfo& gameInfo) {
    // Implementación de sendGameInfo
    if (gameInfo.gamePhase == GamePhase::EndOfMatch) {
        std::cout << "nada." << std::endl;
    }
    throw std::runtime_error("sendGameInfo() no está implementado.");
}

std::string ServerProtocol::recvUsername() {
    uint8_t byte = 0;
    socket.recvall(&byte, sizeof(uint8_t));
    if (byte != BYTE_INIT_MSG) {
        throw std::runtime_error("El byte recibido no es el esperado.");
    }
    uint16_t length = recvLength();
    std::string message;
    message.resize(length);

    socket.recvall(message.data(), sizeof(uint8_t) * length);

    return message;
}

MenuAction ServerProtocol::recvMenuAction() {
    uint8_t byte = 0;
    socket.recvall(&byte, sizeof(uint8_t));

    MenuActionType type = decodeMenuActionType(byte);
    std::string name = "";
    int id_scenary = 0;
    if (type == Create || type == Join) {
        uint16_t length = recvLength();
        name.resize(length);
        socket.recvall(name.data(), sizeof(uint8_t) * length);

        if (type == Create) {
            socket.recvall(&byte, sizeof(uint8_t));
            id_scenary = byte;
        }
    }
    return MenuAction(type, name, id_scenary);
}


LobbyAction ServerProtocol::recvLobbyAction() {
    // Implementación de recvLobbyAction
    uint8_t byte = 0;
    socket.recvall(&byte, sizeof(uint8_t));
    return decodeLobbyAction(byte);
}

GameAction ServerProtocol::recvGameAction() {
    // Implementación de recvGameAction
    return GameAction(Null);
}
