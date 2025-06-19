#include "weapon_info.h"

#include <stdexcept>

WeaponInfo::WeaponInfo(const ServerEntityID server_entt_id, const Weapon weapon,
                       const WeaponState state, const int ammo, const float pos_x,
                       const float pos_y):
        server_entt_id(server_entt_id),
        weapon(weapon),
        state(state),
        ammo(ammo),
        pos_x(pos_x),
        pos_y(pos_y) {}

WeaponInfo::WeaponInfo(const std::vector<uint8_t>& bytes) {
    if (bytes.size() != SIZE_ITEM_INFO) {
        throw std::runtime_error("Error en constructor WeaponInfo(const std::vector<uint8_t>& "
                                 "bytes). El buffer no tiene los bytes esperados.");
    }
    // EntityID (4bytes)
    server_entt_id = Protocol_::getBigEndian32(bytes[0], bytes[1], bytes[2], bytes[3]);

    // Type (1 byte)
    weapon = Protocol_::decodeWeapon(bytes[4]);

    // State
    state = Protocol_::decodeWeaponState(bytes[5]);

    // ammo (2 bytes)
    ammo = Protocol_::getValueBigEndian16(bytes[6], bytes[7]);

    // pos (4 bytes cada uno)
    pos_x = Protocol_::getFloat(bytes[8], bytes[9], bytes[10], bytes[11]);
    pos_y = Protocol_::getFloat(bytes[12], bytes[13], bytes[14], bytes[15]);
}

std::vector<uint8_t> WeaponInfo::toBytes() const {
    std::vector<uint8_t> buffer;

    // EntityID (4 bytes)
    Protocol_::insertBigEndian32(server_entt_id, buffer);
    // Type (1 byte)
    buffer.push_back(Protocol_::encodeWeapon(weapon));
    // State (1 byte)
    buffer.push_back(Protocol_::encodeWeaponState(state));
    // ammo (2bytes)
    Protocol_::insertBigEndian16(ammo, buffer);
    // pos (2 bytes cada uno)
    Protocol_::insertFloat4Bytes(pos_x, buffer);
    Protocol_::insertFloat4Bytes(pos_y, buffer);

    return buffer;
}

SpriteType WeaponInfo::getSpriteType() {
    switch (weapon) {
        case Weapon::None:
        case Weapon::Knife:
            return SpriteType::KNIFE;
        case Weapon::Glock:
            return SpriteType::GLOCK;
        case Weapon::Ak47:
            return SpriteType::AK47;
        case Weapon::M3:
            return SpriteType::M3;
        case Weapon::Awp:
            return SpriteType::AWP;
        case Weapon::Bomb:
            return SpriteType::BOMB;
        default:
            throw std::runtime_error("Falla en WeaponInfo::getSpriteType()");
    }
}
