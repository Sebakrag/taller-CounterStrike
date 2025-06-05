#include "../include/server_protocol.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../../common/constants_protocol.h"

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

void ServerProtocol::sendMatchRoomInfo(const MatchRoomInfo& info) {
    std::vector<uint8_t> buffer;
    buffer.push_back(BYTE_PLAYERS_LIST);
    int size = info.players.size();
    insertBigEndian16(size, buffer);

    for (int i = 0; i < size; i++) {
        insertBigEndian16(info.players[i].username.length(), buffer);
        insertStringBytes(info.players[i].username, buffer);
        buffer.push_back(encodeTeam(info.players[i].team));
    }
    if (info.matchStarted)
        buffer.push_back(BYTE_TRUE);
    else
        buffer.push_back(BYTE_FALSE);

    socket.sendall(buffer.data(), sizeof(uint8_t) * buffer.size());
}

void ServerProtocol::sendGameInfo(const GameInfo& gameInfo) {
    std::vector<uint8_t> buffer;
    buffer.push_back(BYTE_GAME_INFO);
    std::vector<uint8_t> gameInfoBytes = gameInfo.toBytes();
    Protocol_::insertBigEndian16(gameInfoBytes.size(), buffer);
    buffer.insert(buffer.end(), gameInfoBytes.begin(), gameInfoBytes.end());
    socket.sendall(buffer.data(), sizeof(uint8_t) * buffer.size());
}

std::string ServerProtocol::recvUsername() {
    uint8_t byte = 0;
    socket.recvall(&byte, sizeof(uint8_t));
    if (byte != BYTE_INIT_MSG) {
        throw std::runtime_error("El byte recibido no es el esperado.");
    }
    uint16_t length = recvBigEndian16();
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
        uint16_t length = recvBigEndian16();
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
    uint8_t byte = 0;
    socket.recvall(&byte, sizeof(uint8_t));
    return decodeLobbyAction(byte);
}

GameAction ServerProtocol::recvGameAction() {
    GameAction gameAction;
    uint8_t byte = 0;

    socket.recvall(&byte, sizeof(uint8_t));
    GameActionType type = decodeGameActionType(byte);
    gameAction.type = type;
    if (type == BuyWeapon) {
        socket.recvall(&byte, sizeof(uint8_t));
        gameAction.weapon = decodeWeapon(byte);
    } else if (type == BuyAmmo || type == ChangeWeapon) {
        socket.recvall(&byte, 1);
        gameAction.typeWeapon = decodeTypeWeapon(byte);
        gameAction.count_ammo = recvBigEndian16();
    } else if (type == Attack || type == Walk) {
        float dir_x = recvFloatNormalized();
        float dir_y = recvFloatNormalized();
        gameAction.direction = Vec2D(dir_x, dir_y);
    }

    return gameAction;
}
