#include "client_protocol.h"

#include <cstdint>
#include <stdexcept>
#include <vector>

#include "../common/constants_protocol.h"

ClientProtocol::ClientProtocol(const std::string& hostname, const std::string& servname,
                               const std::string& username):
        socket(hostname.c_str(), servname.c_str()), username(username) {}

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
    uint8_t byte;
    socket.recvall(&byte, sizeof(uint8_t));
    if (byte == BYTE_OK)
        return true;
    else if (byte == BYTE_FAIL)
        return false;

    throw std::runtime_error("Error. Se esperaba un byte OK o FAIL y se recibío otra cosa.");
}

std::string ClientProtocol::recvListMatchs() {
    // INSERT_YOUR_CODE
    return "";
}
