#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <cstdint>
#include <string>
#include <vector>

#include "../client/include/model/utils/SpriteType.h"
#include "dtos/MatchInfo.h"
#include "dtos/WindowConfig.h"
#include "utils/EntityType.h"

#include "socket.h"
#include "tile_map.h"
#include "types.h"

#define DECIMAL_SCALE 10000  // 4 cifras significativas
// no me deja ponerle el nombre Protocol ! Segun chat es porque está definido en la libreria de QT
class Protocol_ {
protected:
    Socket socket;

    // constructor para el ClientProtocol
    explicit Protocol_(const std::string& hostname, const std::string& servname);

    // constructor para el ServerProtocol
    explicit Protocol_(Socket&& socketClient);

    // recibe 2 bytes (que representan un numero entero) por el socket, casteando al endianness
    // local
    uint16_t recvBigEndian16();

    uint32_t recvBigEndian32();
    float recvFloat();

    // recibe 3 bytes (correspondiente a un float) por el socket, casteando al endianness local
    float recvFloatNormalized();


public:
    static void insertBigEndian16(uint16_t bytes, std::vector<uint8_t>& array);
    static void insertBigEndian32(uint32_t bytes, std::vector<uint8_t>& array);
    static void insertFloat4Bytes(float value, std::vector<uint8_t>& array);
    // Inserta cada caracter del string (1 byte por caracter) en el array
    static void insertStringBytes(const std::string& string, std::vector<uint8_t>& array);

    /**
     * Inserta un numero float entre -1 y 1 utilizando 3 bytes.
     * 1 byte para el signo. (0=positivo, 1=negativo).
     * 2 bytes para el numero, tomando las primeras cinco cifras
     * (uno de parte entera y cuatro de la parte decimal)
     */
    static void insertFloatNormalized3Bytes(float value, std::vector<uint8_t>& array);

    static uint16_t getValueBigEndian16(uint8_t byte1, uint8_t byte2);
    static uint32_t getBigEndian32(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4);
    static float getFloat(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4);

    static float getFloatNormalized(uint8_t byte1, uint8_t byte2, uint8_t byte3);

    // Codificadores. Devuelven el byte correspondiente al valor del enum.
    static uint8_t encodeBool(bool value);
    static uint8_t encodeTypeWeapon(const TypeWeapon& typeWeapon);
    static uint8_t encodeWeapon(const Weapon& weapon);
    static uint8_t encodeWeaponState(const WeaponState& state);
    static uint8_t encodeMenuActionType(const MenuActionType& gameActionType);
    static uint8_t encodeLobbyAction(const LobbyAction& lobbyAction);
    static uint8_t encodeGameActionType(const GameActionType& gameActionType);
    static uint8_t encodeTeam(const Team&);
    static uint8_t encodeGamePhase(const GamePhase& gamePhase);
    static uint8_t encodePlayerState(const PlayerState& playerState);
    static uint8_t encodePlayerSkin(const PlayerSkin& playerSkin);
    static uint8_t encodeTypeTileMap(const TypeTileMap&);
    static uint8_t encodeBombState(const BombState&);

    // Decodificadores. Devuelven el valor del enum correspondiente al byte.
    static bool decodeBool(uint8_t byte);
    static TypeWeapon decodeTypeWeapon(uint8_t byte);
    static Weapon decodeWeapon(uint8_t byte);
    static WeaponState decodeWeaponState(uint8_t byte);
    static MenuActionType decodeMenuActionType(uint8_t byte);
    static LobbyAction decodeLobbyAction(uint8_t byte);
    static GameActionType decodeGameActionType(uint8_t byte);
    static Team decodeTeam(uint8_t);
    static GamePhase decodeGamePhase(uint8_t byte);
    static PlayerState decodePlayerState(uint8_t byte);
    static PlayerSkin decodePlayerSkin(uint8_t byte);
    static TypeTileMap decodeTypeTileMap(uint8_t byte);
    static BombState decodeBombState(uint8_t byte);

    void shutDown(int how);
};

#endif  // PROTOCOL_H_
