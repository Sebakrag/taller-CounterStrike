#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <cstdint>
#include <string>
#include <vector>

#include "socket.h"
#include "types.h"

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

    // recibe 2 bytes por el socket, casteando al endianness local
    uint16_t recvLength();

    // Codificadores. Devuelven el byte correspondiente al valor del enum.
    uint8_t encodeTypeWeapon(const TypeWeapon& typeWeapon);
    uint8_t encodeWeapon(const Weapon& weapon);
    uint8_t encodeMenuActionType(const MenuActionType& gameActionType);
    uint8_t encodeLobbyAction(const LobbyAction& lobbyAction);
    uint8_t encodeGameActionType(const GameActionType& gameActionType);

    // Decodificadores. Devuelven el valor del enum correspondiente al byte.
    TypeWeapon decodeTypeWeapon(uint8_t byte);
    Weapon decodeWeapon(uint8_t byte);
    MenuActionType decodeMenuActionType(uint8_t byte);
    LobbyAction decodeLobbyAction(uint8_t byte);
    GameActionType decodeGameActionType(uint8_t byte);

public:
    void shutDown(int how);
};

#endif  // PROTOCOL_H_
