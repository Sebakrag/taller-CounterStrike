#include "protocol.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "constants_protocol.h"
#include "socket.h"
#include "types.h"


// constructor para el ClientProtocol
Protocol_::Protocol_(const std::string& hostname, const std::string& servname):
        socket(hostname.c_str(), servname.c_str()) {}

// constructor para el ServerProtocol
Protocol_::Protocol_(Socket&& socketClient): socket(std::move(socketClient)) {}

uint16_t Protocol_::recvBigEndian16() {
    uint16_t number;
    int r = socket.recvall(&number, sizeof(uint16_t));
    if (r != 2) {
        throw std::runtime_error("Error al recibir el number.");
    }
    number = ntohs(number);  // convierto de big endian al endianness local
    return number;
}

uint32_t Protocol_::recvBigEndian32() {
    uint32_t number;
    int r = socket.recvall(&number, sizeof(uint32_t));
    if (r != 4) {
        throw std::runtime_error("Error al recibir el number.");
    }
    number = ntohl(number);  // convierto de big endian al endianness local
    return number;
}

float Protocol_::recvFloat() {
    uint32_t number;
    int r = socket.recvall(&number, sizeof(uint32_t));
    if (r != 4) {
        throw std::runtime_error("Error al recibir el number.");
    }
    number = ntohl(number);  // convierto de big endian al endianness local

    float value;
    std::memcpy(&value, &number, sizeof(float));
    return value;
}

void Protocol_::insertBigEndian16(uint16_t bytes, std::vector<uint8_t>& array) {
    uint16_t bytes_big_endian = htons(bytes);  // paso a big endian

    uint8_t* bytes_be_ptr = reinterpret_cast<uint8_t*>(&bytes_big_endian);

    array.push_back(bytes_be_ptr[0]);  // Primer byte en big endian
    array.push_back(bytes_be_ptr[1]);  // Segundo byte en big endian
}

void Protocol_::insertBigEndian32(uint32_t bytes, std::vector<uint8_t>& array) {
    uint32_t bytes_big_endian = htonl(bytes);  // paso a big endian

    uint8_t* bytes_be_ptr = reinterpret_cast<uint8_t*>(&bytes_big_endian);

    array.push_back(bytes_be_ptr[0]);  // Primer byte en big endian
    array.push_back(bytes_be_ptr[1]);  // Segundo byte en big endian
    array.push_back(bytes_be_ptr[2]);  // tercer byte en big endian
    array.push_back(bytes_be_ptr[3]);  // cuarto byte en big endian
}

void Protocol_::insertFloat4Bytes(float value, std::vector<uint8_t>& array) {
    // Interpretar los bits del float como un entero de 32 bits sin alterar
    uint32_t bytes;
    std::memcpy(&bytes, &value, sizeof(float));  // Copia binaria segura

    // Convertir a big-endian
    uint32_t big_endian = htonl(bytes);
    uint8_t* bytes_be_ptr = reinterpret_cast<uint8_t*>(&big_endian);

    array.push_back(bytes_be_ptr[0]);  // Primer byte en big endian
    array.push_back(bytes_be_ptr[1]);  // Segundo byte en big endian
    array.push_back(bytes_be_ptr[2]);  // tercer byte en big endian
    array.push_back(bytes_be_ptr[3]);  // cuarto byte en big endian
}

void Protocol_::insertStringBytes(const std::string& string, std::vector<uint8_t>& array) {
    std::copy(string.begin(), string.end(), std::back_inserter(array));
}

void Protocol_::insertFloatNormalized3Bytes(float value, std::vector<uint8_t>& array) {
    if (value > 1.0f)
        value = 1.0f;
    if (value < -1.0f)
        value = -1.0f;

    // 1. Signo
    uint8_t sign = 0;  // positivo
    if (value < 0.0f)
        sign = 1;  // negativo

    array.push_back(sign);

    // 2. numero .
    uint16_t n = static_cast<uint16_t>(std::abs(value) *
                                       DECIMAL_SCALE);  // corro la coma 4 lugares a la derecha.

    // cargo la parte entera del numero obtenido
    insertBigEndian16(n, array);
}

float Protocol_::recvFloatNormalized() {
    float value = 0;
    uint8_t sign = 0;
    int r = socket.recvall(&sign, sizeof(uint8_t));
    if (r != 1) {
        throw std::runtime_error("Error al recibir el signo del float.");
    }
    uint16_t mantissa = recvBigEndian16();

    value = static_cast<float>(mantissa) / DECIMAL_SCALE;

    if (sign == 1) {
        value = value * (-1);
    }
    //    std::cout << "Valor recibido: " << std::to_string(value) << std::endl;

    return value;
}

uint16_t Protocol_::getValueBigEndian16(uint8_t byte1, uint8_t byte2) {
    uint16_t value;
    uint8_t bytes[2] = {byte1, byte2};
    std::memcpy(&value, bytes, sizeof(uint16_t));
    value = ntohs(value);  // convierto de big endian al endianness local
    return value;
}

uint32_t Protocol_::getBigEndian32(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) {
    uint32_t value;
    uint8_t bytes[4] = {byte1, byte2, byte3, byte4};
    std::memcpy(&value, bytes, sizeof(uint32_t));
    value = ntohl(value);  // convierto de big endian al endianness local
    return value;
}

float Protocol_::getFloat(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) {
    uint32_t value_bytes;
    uint8_t bytes[4] = {byte1, byte2, byte3, byte4};
    std::memcpy(&value_bytes, bytes, sizeof(uint32_t));
    value_bytes = ntohl(value_bytes);  // convierto de big endian al endianness local

    float value;
    std::memcpy(&value, &value_bytes, sizeof(float));
    return value;
}

float Protocol_::getFloatNormalized(uint8_t byte1, uint8_t byte2, uint8_t byte3) {
    float value = 0;

    uint16_t mantissa = getValueBigEndian16(byte2, byte3);

    value = static_cast<float>(mantissa) / DECIMAL_SCALE;

    if (byte1 == 1) {
        value *= (-1);
    }
    return value;
}

// Codificadores
uint8_t Protocol_::encodeBool(bool value) {
    if (value)
        return BYTE_TRUE;
    return BYTE_FALSE;
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
        case Weapon::Knife:
            return BYTE_WEAPON_KNIFE;
        case Weapon::Bomb:
            return BYTE_WEAPON_BOMB;
        default:
            return BYTE_WEAPON_NONE;  // Valor predeterminado en caso de error
    }
}

uint8_t Protocol_::encodeWeaponState(const WeaponState& state) {
    switch (state) {
        case WeaponState::DROPPED:
            return BYTE_WEAPON_STATE_DROPPED;
        case WeaponState::EQUIPPED:
            return BYTE_WEAPON_STATE_EQUIPPED;
        case WeaponState::HIDDEN:
            return BYTE_WEAPON_STATE_HIDDEN;
        default:
            return BYTE_WEAPON_STATE_NONE;  // Valor predeterminado en caso de error
    }
}

uint8_t Protocol_::encodeAmmoType(const AmmoType& ammoType) {
    switch (ammoType) {
        case AmmoType::Primary:
            return BYTE_PRIMARY_AMMO;
        case AmmoType::Secondary:
            return BYTE_SECONDARY_AMMO;
        default:
            return BYTE_NONE_AMMO;  // Valor predeterminado en caso de error
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
        case MenuActionType::ListScenarios:
            return BYTE_SCENARIOS_LIST;
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
        case GameActionType::Rotate:
            return BYTE_ROTATE;
        case GameActionType::DefuseBomb:
            return BYTE_DEFUSE_BOMB;
        case GameActionType::ExitMatch:
            return BYTE_EXIT_MATCH;
        default:
            throw std::runtime_error(
                    "Error. Tipo de acción de juego desconocida. No se puede codificar");
    }
}

uint8_t Protocol_::encodeTeam(const Team& team) {
    switch (team) {
        case Team::Terrorist:
            return BYTE_TEAM_TERRORIST;
        case Team::CounterTerrorist:
            return BYTE_TEAM_COUNTERTERRORIST;
        default:
            throw std::invalid_argument("Team inválido");
    }
}

uint8_t Protocol_::encodeGamePhase(const GamePhase& gamePhase) {
    switch (gamePhase) {
        case GamePhase::Preparation:
            return BYTE_PHASE_PREPARATION;
        case GamePhase::Combat:
            return BYTE_PHASE_COMBAT;
        case GamePhase::EndOfMatch:
            return BYTE_PHASE_END_OF_MATCH;
        default:
            throw std::runtime_error("Error. Fase de juego desconocida. No se puede codificar");
    }
}

uint8_t Protocol_::encodePlayerState(const PlayerState& playerState) {
    switch (playerState) {
        case PlayerState::Idle:
            return BYTE_STATE_IDLE;
        case PlayerState::Walking:
            return BYTE_STATE_WALKING;
        case PlayerState::Attacking:
            return BYTE_STATE_ATTACKING;
        case PlayerState::TakingDamage:
            return BYTE_STATE_TAKING_DAMAGE;
        case PlayerState::PickingUp:
            return BYTE_STATE_PICKING_UP;
        case PlayerState::Dead:
            return BYTE_STATE_DEAD;
        default:
            throw std::runtime_error(
                    "Error. Estado del jugador desconocido. No se puede codificar");
    }
}

uint8_t Protocol_::encodePlayerSkin(const PlayerSkin& playerSkin) {
    switch (playerSkin) {
        case PlayerSkin::Terrorist1:
            return BYTE_SKIN_TERRORIST_1;
        case PlayerSkin::Terrorist2:
            return BYTE_SKIN_TERRORIST_2;
        case PlayerSkin::Terrorist3:
            return BYTE_SKIN_TERRORIST_3;
        case PlayerSkin::Terrorist4:
            return BYTE_SKIN_TERRORIST_4;
        case PlayerSkin::CounterTerrorist1:
            return BYTE_SKIN_COUNTERTERRORIST_1;
        case PlayerSkin::CounterTerrorist2:
            return BYTE_SKIN_COUNTERTERRORIST_2;
        case PlayerSkin::CounterTerrorist3:
            return BYTE_SKIN_COUNTERTERRORIST_3;
        case PlayerSkin::CounterTerrorist4:
            return BYTE_SKIN_COUNTERTERRORIST_4;
        default:
            throw std::runtime_error("Error. Skin de jugador desconocida. No se puede codificar");
    }
}


uint8_t Protocol_::encodeTypeTileMap(const TypeTileMap& type) {
    if (type == TypeTileMap::Aztec)
        return BYTE_MAP_AZTEC;
    if (type == TypeTileMap::Desert)
        return BYTE_MAP_DESERT;
    else
        return BYTE_MAP_TRAINING;
}

uint8_t Protocol_::encodeBombState(const BombState& state) {
    switch (state) {
        case BombState::Dropped:
            return BYTE_BOMB_DROPPED;
        case BombState::Equipped:
            return BYTE_BOMB_EQUIPPED;
        case BombState::Hidden:
            return BYTE_BOMB_HIDDEN;
        case BombState::Planted:
            return BYTE_BOMB_PLANTED;
        case BombState::Exploded:
            return BYTE_BOMB_EXPLODED;
        case BombState::Defused:
            return BYTE_BOMB_DEFUSED;
        default:
            throw std::runtime_error("Error. Estado de bomba desconocido. No se puede codificar");
    }
}

// Decodificadores.
//----------------------------------------------------------------------------------
bool Protocol_::decodeBool(uint8_t byte) {
    if (byte == BYTE_TRUE)
        return true;
    else if (byte == BYTE_FALSE)
        return false;

    throw std::runtime_error("Error. El byte no representa un booleano");
}
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
        case BYTE_WEAPON_KNIFE:
            return Weapon::Knife;
        case BYTE_WEAPON_BOMB:
            return Weapon::Bomb;
        default:
            throw std::runtime_error("Error. Arma desconocida. No se puede decodificar");
    }
}

AmmoType Protocol_::decodeAmmoType(uint8_t byte) {
    switch (byte) {
        case BYTE_PRIMARY_AMMO:
            return AmmoType::Primary;
        case BYTE_SECONDARY_AMMO:
            return AmmoType::Secondary;
        case BYTE_NONE_AMMO:
            return AmmoType::None;
        default:
            throw std::runtime_error("Error. Ammo type desconocido. No se puede decodificar");
    }
}

WeaponState Protocol_::decodeWeaponState(uint8_t byte) {
    switch (byte) {
        case BYTE_WEAPON_STATE_DROPPED:
            return WeaponState::DROPPED;
        case BYTE_WEAPON_STATE_EQUIPPED:
            return WeaponState::EQUIPPED;
        case BYTE_WEAPON_STATE_HIDDEN:
            return WeaponState::HIDDEN;
        default:
            throw std::runtime_error("Error. Estado de arma desconocida. No se puede decodificar");
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
        case BYTE_SCENARIOS_LIST:
            return MenuActionType::ListScenarios;
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
        case BYTE_ROTATE:
            return GameActionType::Rotate;
        case BYTE_DEFUSE_BOMB:
            return GameActionType::DefuseBomb;
        case BYTE_EXIT_MATCH:
            return GameActionType::ExitMatch;
        default:
            throw std::runtime_error(
                    "Error. Tipo de acción de juego desconocida. No se puede decodificar");
    }
}
Team Protocol_::decodeTeam(uint8_t byte) {
    switch (byte) {
        case BYTE_TEAM_TERRORIST:
            return Team::Terrorist;
        case BYTE_TEAM_COUNTERTERRORIST:
            return Team::CounterTerrorist;
        default:
            throw std::invalid_argument("Byte inválido para Team");
    }
}
GamePhase Protocol_::decodeGamePhase(uint8_t byte) {
    switch (byte) {
        case BYTE_PHASE_PREPARATION:
            return GamePhase::Preparation;
        case BYTE_PHASE_COMBAT:
            return GamePhase::Combat;
        case BYTE_PHASE_END_OF_MATCH:
            return GamePhase::EndOfMatch;
        default:
            throw std::runtime_error("Error. Fase de juego desconocida. No se puede decodificar");
    }
}

PlayerState Protocol_::decodePlayerState(uint8_t byte) {
    switch (byte) {
        case BYTE_STATE_IDLE:
            return PlayerState::Idle;
        case BYTE_STATE_WALKING:
            return PlayerState::Walking;
        case BYTE_STATE_ATTACKING:
            return PlayerState::Attacking;
        case BYTE_STATE_TAKING_DAMAGE:
            return PlayerState::TakingDamage;
        case BYTE_STATE_PICKING_UP:
            return PlayerState::PickingUp;
        case BYTE_STATE_DEAD:
            return PlayerState::Dead;
        default:
            std::cout << "Byte desconocido: 0x" << std::hex << static_cast<int>(byte) << std::dec
                      << std::endl;
            throw std::runtime_error(
                    "Error. Estado del jugador desconocido. No se puede decodificar");
    }
}

PlayerSkin Protocol_::decodePlayerSkin(uint8_t byte) {
    switch (byte) {
        case BYTE_SKIN_TERRORIST_1:
            return PlayerSkin::Terrorist1;
        case BYTE_SKIN_TERRORIST_2:
            return PlayerSkin::Terrorist2;
        case BYTE_SKIN_TERRORIST_3:
            return PlayerSkin::Terrorist3;
        case BYTE_SKIN_TERRORIST_4:
            return PlayerSkin::Terrorist4;
        case BYTE_SKIN_COUNTERTERRORIST_1:
            return PlayerSkin::CounterTerrorist1;
        case BYTE_SKIN_COUNTERTERRORIST_2:
            return PlayerSkin::CounterTerrorist2;
        case BYTE_SKIN_COUNTERTERRORIST_3:
            return PlayerSkin::CounterTerrorist3;
        case BYTE_SKIN_COUNTERTERRORIST_4:
            return PlayerSkin::CounterTerrorist4;
        default:
            throw std::runtime_error("Error. Skin de jugador desconocida. No se puede decodificar");
    }
}

TypeTileMap Protocol_::decodeTypeTileMap(uint8_t byte) {
    switch (byte) {
        case BYTE_MAP_AZTEC:
            return TypeTileMap::Aztec;
        case BYTE_MAP_DESERT:
            return TypeTileMap::Desert;
        case BYTE_MAP_TRAINING:
            return TypeTileMap::Training;
        default:
            throw std::runtime_error("Error. Tipo de map desconcido. No se puede decodificar");
    }
}

BombState Protocol_::decodeBombState(uint8_t byte) {
    switch (byte) {
        case BYTE_BOMB_DROPPED:
            return BombState::Dropped;
        case BYTE_BOMB_EQUIPPED:
            return BombState::Equipped;
        case BYTE_BOMB_HIDDEN:
            return BombState::Hidden;
        case BYTE_BOMB_PLANTED:
            return BombState::Planted;
        case BYTE_BOMB_EXPLODED:
            return BombState::Exploded;
        case BYTE_BOMB_DEFUSED:
            return BombState::Defused;
        default:
            throw std::runtime_error("Error. Estado de bomba desconcido. No se puede decodificar");
    }
}

void Protocol_::shutDown(int how) { socket.shutdown(how); }
