#include "../include/server_protocol.h"

#include <cstdint>
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

void ServerProtocol::sendMatchInfo(const MatchInfo& matchInfo) {
    std::vector<uint8_t> buffer;
    buffer.push_back(BYTE_MATCH_INFO);

    // cargo name
    insertBigEndian16(matchInfo.name.length(), buffer);
    insertStringBytes(matchInfo.name, buffer);

    // cargo win_config
    insertBigEndian16(matchInfo.win_config.width, buffer);
    insertBigEndian16(matchInfo.win_config.height, buffer);
    insertBigEndian32(matchInfo.win_config.flags, buffer);

    // 3) FOVConfig
    // isActive (1 byte)
    buffer.push_back(encodeBool(matchInfo.fovConfig.isActive));
    // screenWidth & screenHeight & circleRadius (2 bytes each)
    insertBigEndian16(matchInfo.fovConfig.screenWidth, buffer);
    insertBigEndian16(matchInfo.fovConfig.screenHeight, buffer);
    insertBigEndian16(matchInfo.fovConfig.circleRadius, buffer);
    // fovAngle, visibilityDistance, transparency (4 bytes cada uno)
    insertFloat4Bytes(matchInfo.fovConfig.fovAngle, buffer);
    insertFloat4Bytes(matchInfo.fovConfig.visibilityDistance, buffer);
    insertFloat4Bytes(matchInfo.fovConfig.transparency, buffer);

    // cargo tilemap
    std::vector<uint8_t> tilemap_bytes = matchInfo.tileMap.toBytes();

    insertBigEndian32(tilemap_bytes.size(), buffer);
    for (uint8_t& b: tilemap_bytes) {
        buffer.push_back(b);
    }
    // numero de jugadores
    insertBigEndian16(matchInfo.numPlayers, buffer);

    // playerInfo
    std::vector<uint8_t> playerInfobytes = matchInfo.localPlayerInfo.toBytes();
    insertBigEndian16(playerInfobytes.size(), buffer);
    for (uint8_t& b: playerInfobytes) {
        buffer.push_back(b);
    }
    socket.sendall(buffer.data(), sizeof(uint8_t) * buffer.size());
}


void ServerProtocol::sendListOfMatchs(std::list<std::string> matchs_names) {
    std::vector<uint8_t> buffer;
    std::string msg;
    for (const auto& match: matchs_names) {
        msg += match + "\n";
    }
    buffer.push_back(BYTE_MATCH_LIST);

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
        buffer.push_back(encodeBool(info.players[i].is_player_host));
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
    } else if (type == Rotate) {
        gameAction.angle = recvFloat();
    }

    return gameAction;
}
