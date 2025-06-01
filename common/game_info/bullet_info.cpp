#include "bullet_info.h"

BulletInfo::BulletInfo(int id, Weapon weapon, int pos_x, int pos_y, const Vec2D& direction):
        id(id), weapon(weapon), pos_x(pos_x), pos_y(pos_y), direction(direction) {}

BulletInfo::BulletInfo(const BulletInfo& other):
        id(other.id),
        weapon(other.weapon),
        pos_x(other.pos_x),
        pos_y(other.pos_y),
        direction(other.direction) {}

BulletInfo& BulletInfo::operator=(const BulletInfo& other) {
    if (this != &other) {
        id = other.id;
        weapon = other.weapon;
        pos_x = other.pos_x;
        pos_y = other.pos_y;
        direction = other.direction;
    }
    return *this;
}


BulletInfo::BulletInfo(const std::vector<uint8_t>& bytes) {
    // ID (2 bytes)
    id = Protocol_::getValueBigEndian16(bytes[0], bytes[1]);

    // Weapon (1 byte)
    weapon = Protocol_::decodeWeapon(bytes[2]);

    // Position (2 bytes cada uno)
    pos_x = Protocol_::getValueBigEndian16(bytes[3], bytes[4]);
    pos_y = Protocol_::getValueBigEndian16(bytes[5], bytes[6]);

    // Direction (3 bytes cada componente)
    direction.setX(Protocol_::getFloatNormalized(bytes[7], bytes[8], bytes[9]));
    direction.setY(Protocol_::getFloatNormalized(bytes[10], bytes[11], bytes[12]));
}

std::vector<uint8_t> BulletInfo::toBytes() const {
    std::vector<uint8_t> buffer;

    // ID (2 bytes)
    Protocol_::insertBigEndian16(id, buffer);

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
