#include "protocol.h"

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "constants_protocol.h"
#include "socket.h"


// constructor para el ClientProtocol
Protocol_::Protocol_(const std::string& hostname, const std::string& servname):
        socket(hostname.c_str(), servname.c_str()) {}

// constructor para el ServerProtocol
Protocol_::Protocol_(Socket&& socketClient): socket(std::move(socketClient)) {}

uint16_t Protocol_::recvLength() {
    uint16_t length;
    int r = socket.recvall(&length, sizeof(uint16_t));
    if (r != 2) {
        throw std::runtime_error("Error al recibir el length.");
    }
    length = ntohs(length);  // convierto de big endian al endianness local
    return length;
}


void Protocol_::insertBigEndian16(uint16_t bytes, std::vector<uint8_t>& array) {
    uint16_t bytes_big_endian = htons(bytes);  // paso a big endian

    uint8_t* bytes_be_ptr = reinterpret_cast<uint8_t*>(&bytes_big_endian);

    array.push_back(bytes_be_ptr[0]);  // Primer byte en big endian
    array.push_back(bytes_be_ptr[1]);  // Segundo byte en big endian
}

void Protocol_::insertStringBytes(const std::string& string, std::vector<uint8_t>& array) {
    std::copy(string.begin(), string.end(), std::back_inserter(array));
}


uint8_t Protocol_::encodeTypeWeapon(const TypeWeapon& typeWeapon) {
    switch (typeWeapon) {
        case TypeWeapon::Primary:
            return BYTE_TYPE_PRIMARY;
        case TypeWeapon::Secondary:
            return BYTE_TYPE_SECONDARY;
        case TypeWeapon::Knife:
            return BYTE_TYPE_KNIFE;
        case TypeWeapon::Bomb:
            return BYTE_TYPE_BOMB;
        default:
            throw std::runtime_error("Error. TypeWeapon desconocida. No se puede codificar");
    }
}

uint8_t Protocol_::encodeWeapon(const Weapon& weapon) {
    switch (weapon) {
        case Weapon::Glock:
            return BYTE_WEAPON_GLOCK;
        case Weapon::Ak47:
            return BYTE_WEAPON_AK_47;
        case Weapon::M3:
            return BYTE_WEAPON_M3;
        case Weapon::Awp:
            return BYTE_WEAPON_AWP;
        default:
            return BYTE_WEAPON_NONE;  // Valor predeterminado en caso de error
    }
}

uint8_t Protocol_::encodeMenuActionType(const MenuActionType& gameActionType) {
    switch (gameActionType) {
        case MenuActionType::Exit:
            return BYTE_EXIT;
        case MenuActionType::Create:
            return BYTE_CREATE_MATCH;
        case MenuActionType::Join:
            return BYTE_JOIN_MATCH;
        case MenuActionType::List:
            return BYTE_LIST_MATCHS;
        default:
            throw std::runtime_error("Error. Tipo de acción desconocida. No se puede codificar");
    }
}

uint8_t Protocol_::encodeLobbyAction(const LobbyAction& lobbyAction) {
    switch (lobbyAction) {
        case LobbyAction::QuitMatch:
            return BYTE_QUIT_MATCH;
        case LobbyAction::StartMatch:
            return BYTE_START_MATCH;
        case LobbyAction::ListPlayers:
            return BYTE_LIST_PLAYERS;
        default:
            throw std::runtime_error("Error. Acción de lobby desconocida. No se puede codificar");
    }
}

uint8_t Protocol_::encodeGameActionType(const GameActionType& gameActionType) {
    switch (gameActionType) {
        case GameActionType::BuyWeapon:
            return BYTE_BUY_WEAPON;
        case GameActionType::BuyAmmo:
            return BYTE_BUY_AMMO;
        case GameActionType::Attack:
            return BYTE_ATTACK;
        case GameActionType::Walk:
            return BYTE_WALK;
        case GameActionType::ChangeWeapon:
            return BYTE_CHANGE_WEAPON;
        case GameActionType::PickUp:
            return BYTE_PICK_UP;
        default:
            throw std::runtime_error(
                    "Error. Tipo de acción de juego desconocida. No se puede codificar");
    }
}

// Decodificadores.
//------------------

TypeWeapon Protocol_::decodeTypeWeapon(uint8_t byte) {
    switch (byte) {
        case BYTE_TYPE_PRIMARY:
            return TypeWeapon::Primary;
        case BYTE_TYPE_SECONDARY:
            return TypeWeapon::Secondary;
        case BYTE_TYPE_KNIFE:
            return TypeWeapon::Knife;
        case BYTE_TYPE_BOMB:
            return TypeWeapon::Bomb;
        default:
            throw std::runtime_error("Error. Tipo de arma desconocido. No se puede decodificar");
    }
}

Weapon Protocol_::decodeWeapon(uint8_t byte) {
    switch (byte) {
        case BYTE_WEAPON_NONE:
            return Weapon::None;
        case BYTE_WEAPON_GLOCK:
            return Weapon::Glock;
        case BYTE_WEAPON_AK_47:
            return Weapon::Ak47;
        case BYTE_WEAPON_M3:
            return Weapon::M3;
        case BYTE_WEAPON_AWP:
            return Weapon::Awp;
        default:
            throw std::runtime_error("Error. Arma desconocida. No se puede decodificar");
    }
}

MenuActionType Protocol_::decodeMenuActionType(uint8_t byte) {
    switch (byte) {
        case BYTE_CREATE_MATCH:
            return MenuActionType::Create;
        case BYTE_JOIN_MATCH:
            return MenuActionType::Join;
        case BYTE_LIST_MATCHS:
            return MenuActionType::List;
        case BYTE_EXIT:
            return MenuActionType::Exit;
        default:
            throw std::runtime_error(
                    "Error. Tipo de acción de menú desconocida. No se puede decodificar");
    }
}

LobbyAction Protocol_::decodeLobbyAction(uint8_t byte) {
    switch (byte) {
        case BYTE_QUIT_MATCH:
            return LobbyAction::QuitMatch;
        case BYTE_START_MATCH:
            return LobbyAction::StartMatch;
        case BYTE_LIST_PLAYERS:
            return LobbyAction::ListPlayers;
        default:
            throw std::runtime_error("Error. Acción de lobby desconocida. No se puede decodificar");
    }
}

GameActionType Protocol_::decodeGameActionType(uint8_t byte) {
    switch (byte) {
        case BYTE_BUY_WEAPON:
            return GameActionType::BuyWeapon;
        case BYTE_BUY_AMMO:
            return GameActionType::BuyAmmo;
        case BYTE_ATTACK:
            return GameActionType::Attack;
        case BYTE_WALK:
            return GameActionType::Walk;
        case BYTE_CHANGE_WEAPON:
            return GameActionType::ChangeWeapon;
        case BYTE_PICK_UP:
            return GameActionType::PickUp;
        default:
            throw std::runtime_error(
                    "Error. Tipo de acción de juego desconocida. No se puede decodificar");
    }
}

void Protocol_::shutDown(int how) { socket.shutdown(how); }
