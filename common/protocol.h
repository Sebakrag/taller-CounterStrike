#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <cstdint>
#include <string>
#include <vector>

#include "socket.h"
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

    /**
     * Inserta un numero de 2 bytes en formato big-endian dentro del array.
     */
    void insertBigEndian16(uint16_t bytes, std::vector<uint8_t>& array);

    // Inserta cada caracter del string (1 byte por caracter) en el array
    void insertStringBytes(const std::string& string, std::vector<uint8_t>& array);

    /**
     * Inserta un numero float entre -1 y 1 utilizando 3 bytes.
     * 1 byte para el signo. (0=positivo, 1=negativo).
     * 2 bytes para el numero, tomando las primeras cinco cifras
     * (uno de parte entera y cuatro de la parte decimal)
     */
    void insertFloatNormalized3Bytes(float value, std::vector<uint8_t>& array);

    // recibe 2 bytes (que representan un numero entero) por el socket, casteando al endianness
    // local
    uint16_t recvBigEndian16();

    // recibe 3 bytes (correspondiente a un float) por el socket, casteando al endianness local
    float recvFloatNormalized();

public:
    // Codificadores. Devuelven el byte correspondiente al valor del enum.
    static uint8_t encodeTypeWeapon(const TypeWeapon& typeWeapon);
    static uint8_t encodeWeapon(const Weapon& weapon);
    static uint8_t encodeMenuActionType(const MenuActionType& gameActionType);
    static uint8_t encodeLobbyAction(const LobbyAction& lobbyAction);
    static uint8_t encodeGameActionType(const GameActionType& gameActionType);
    static uint8_t encodeTeam(const Team&);
    static uint8_t encodeGamePhase(const GamePhase& gamePhase);
    static uint8_t encodePlayerState(const PlayerState& playerState);

    // Decodificadores. Devuelven el valor del enum correspondiente al byte.
    static TypeWeapon decodeTypeWeapon(uint8_t byte);
    static Weapon decodeWeapon(uint8_t byte);
    static MenuActionType decodeMenuActionType(uint8_t byte);
    static LobbyAction decodeLobbyAction(uint8_t byte);
    static GameActionType decodeGameActionType(uint8_t byte);
    static Team decodeTeam(uint8_t);
    static GamePhase decodeGamePhase(uint8_t byte);
    static PlayerState decodePlayerState(uint8_t byte);

    void shutDown(int how);
};

#endif  // PROTOCOL_H_
