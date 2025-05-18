#include "client_protocol.h"

#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <arpa/inet.h>

#include "../common/constants_protocol.h"

ClientProtocol::ClientProtocol(const std::string& hostname, const std::string& servname,
                               const std::string& username):
        Protocol_(hostname, servname), username(username) {
    // sendUserName(username);
}


void ClientProtocol::sendUserName(const std::string& username) {
    std::vector<uint8_t> buffer;

    buffer.push_back(BYTE_INIT_MSG);
    insertBigEndian16(username.length(), buffer);
    insertStringBytes(username, buffer);

    socket.sendall(buffer.data(), sizeof(uint8_t) * buffer.size());
}


void ClientProtocol::sendMenuAction(const MenuAction& action) {
    std::vector<uint8_t> buffer;
    int length = action.name_match.length();

    buffer.push_back(encodeMenuActionType(action.type));

    if (action.type == Create || action.type == Join) {
        insertBigEndian16(length, buffer);
        insertStringBytes(action.name_match, buffer);
        if (action.type == Create)
            buffer.push_back(action.id_scenary);
    }
    socket.sendall(buffer.data(), sizeof(uint8_t) * buffer.size());
}

void ClientProtocol::sendLobbyAction(const LobbyAction& action) {
    std::vector<uint8_t> buffer;

    if (action == LobbyAction::QuitMatch) {
        buffer.push_back(BYTE_QUIT_MATCH);
    } else if (action == LobbyAction::StartMatch) {
        buffer.push_back(BYTE_START_MATCH);
    } else {
        buffer.push_back(BYTE_LIST_PLAYERS);
    }
    socket.sendall(buffer.data(), sizeof(uint8_t) * buffer.size());
}

void ClientProtocol::sendGameAction(const GameAction& gameAction) {
    std::vector<uint8_t> buffer;

    buffer.push_back(encodeGameActionType(gameAction.type));

    if (gameAction.type == BuyWeapon) {
        buffer.push_back(encodeWeapon(gameAction.weapon));
    } else if (gameAction.type == BuyAmmo) {
        buffer.push_back(encodeTypeWeapon(gameAction.typeWeapon));
        insertBigEndian16(gameAction.count_ammo, buffer);
    } else if (gameAction.type == Attack) {
        // insertar 2 bytes para float x
        // insertar 2 bytes para float y
    } else if (gameAction.type == Walk) {
        // insertar 2 bytes para float x
        // insertar 2 bytes para float y
    } else if (gameAction.type == ChangeWeapon) {
        buffer.push_back(encodeTypeWeapon(gameAction.typeWeapon));
    }

    socket.sendall(buffer.data(), sizeof(uint8_t) * buffer.size());
}

bool ClientProtocol::recvConfirmation() {
    uint8_t byte = 0;
    socket.recvall(&byte, sizeof(uint8_t));
    if (byte == BYTE_OK)
        return true;
    else if (byte == BYTE_FAIL)
        return false;

    throw std::runtime_error("Error. Se esperaba un byte OK o FAIL y se recibío otra cosa.");
}

std::vector<std::string> ClientProtocol::recvListMatchs() {
    std::string message;
    uint8_t byte = 0;

    int r = socket.recvall(&byte, sizeof(uint8_t));
    if (r == 0) {
        throw std::runtime_error(
                "Error. No se recibió ningún byte. Probablemente se cerró el Server.");
    }
    if (byte != BYTE_MATCH_LIST) {
        throw std::runtime_error(
                "Error. Se esperaba recibir la lista de partidas y llegó otro mensaje.");
    }

    // recibo el length
    uint16_t length = recvLength();

    // recibo el string
    message.resize(length);
    socket.recvall(message.data(), sizeof(uint8_t) * length);

    // Divido el mensaje por '\n' y retorno como vector
    std::vector<std::string> matchNames;
    std::stringstream ss(message);
    std::string line;

    while (std::getline(ss, line, '\n')) {
        if (!line.empty()) {
            matchNames.push_back(line);
        }
    }

    return matchNames;
}

std::vector<std::string> ClientProtocol::recvListPlayers() {
    std::vector<std::string> list;
    std::cout << "NO ESTÁ IMPLMENTADO! ClientProtocol::recvListPlayers()" << std::endl;
    return list;
}

GameInfo ClientProtocol::recvGameInfo() {
    GameInfo gameInfo(GamePhase::Preparation, std::vector<PlayerInfo>());
    std::cout << "NO ESTÁ IMPLMENTADO! ClientProtocol::recvGameInfo()" << std::endl;
    return gameInfo;
}
