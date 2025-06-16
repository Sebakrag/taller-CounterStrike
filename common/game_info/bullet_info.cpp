#include "bullet_info.h"

#include <stdexcept>

BulletInfo::BulletInfo(const ServerEntityID id, const Weapon weapon, const int pos_x,
                       const int pos_y, const Vec2D& direction):
        id(id), weapon(weapon), pos_x(pos_x), pos_y(pos_y), direction(direction) {}

BulletInfo::BulletInfo(const std::vector<uint8_t>& bytes) {
    // Chequear el size y lanzar excepcion
    if (bytes.size() != SIZE_BULLET_INFO) {
        throw std::runtime_error("Error en constructor BulletInfo(const std::vector<uint8_t>& "
                                 "bytes). El buffer no tiene los bytes esperados.");
    }
    // ID (4 bytes)
    id = Protocol_::getBigEndian32(bytes[0], bytes[1], bytes[2], bytes[3]);

    // Weapon (1 byte)
    weapon = Protocol_::decodeWeapon(bytes[4]);

    // Position (2 bytes cada uno)
    pos_x = Protocol_::getValueBigEndian16(bytes[5], bytes[6]);
    pos_y = Protocol_::getValueBigEndian16(bytes[7], bytes[8]);

    // Direction (3 bytes cada componente)
    direction.setX(Protocol_::getFloatNormalized(bytes[9], bytes[10], bytes[11]));
    direction.setY(Protocol_::getFloatNormalized(bytes[12], bytes[13], bytes[14]));
}

std::vector<uint8_t> BulletInfo::toBytes() const {
    std::vector<uint8_t> buffer;

    // ID (4 bytes)
    Protocol_::insertBigEndian32(id, buffer);

    // Weapon (1 byte)
    buffer.push_back(Protocol_::encodeWeapon(weapon));

    // Position (2 bytes cada uno)
    Protocol_::insertBigEndian16(pos_x, buffer);
    Protocol_::insertBigEndian16(pos_y, buffer);

    // Direction (3 bytes cada componente)
    Protocol_::insertFloatNormalized3Bytes(direction.getX(), buffer);
    Protocol_::insertFloatNormalized3Bytes(direction.getY(), buffer);

    return buffer;
}

SpriteType BulletInfo::getSpriteType() { return SpriteType::BULLET; }
