#include "item_info.h"

#include <stdexcept>

ItemInfo::ItemInfo(unsigned int server_entt_id, TypeItem type, int ammo, int pos_x, int pos_y):
        server_entt_id(server_entt_id), type(type), ammo(ammo), pos_x(pos_x), pos_y(pos_y) {}

ItemInfo::ItemInfo(const ItemInfo& other):
        server_entt_id(other.server_entt_id),
        type(other.type),
        ammo(other.ammo),
        pos_x(other.pos_x),
        pos_y(other.pos_y) {}

ItemInfo& ItemInfo::operator=(const ItemInfo& other) {
    if (this != &other) {
        server_entt_id = other.server_entt_id;
        type = other.type;
        ammo = other.ammo;
        pos_x = other.pos_x;
        pos_y = other.pos_y;
    }
    return *this;
}

ItemInfo::ItemInfo(const std::vector<uint8_t>& bytes) {
    if (bytes.size() != SIZE_ITEM_INFO) {
        throw std::runtime_error("Error en constructor ItemInfo(const std::vector<uint8_t>& "
                                 "bytes). El buffer no tiene los bytes esperados.");
    }
    // EntityID (4bytes)
    server_entt_id = Protocol_::getBigEndian32(bytes[0], bytes[1], bytes[2], bytes[3]);

    // Type (1 byte)
    type = Protocol_::decodeTypeItem(bytes[4]);

    // ammo (2 bytes)
    ammo = Protocol_::getValueBigEndian16(bytes[5], bytes[6]);

    // pos (2 bytes cada uno)
    pos_x = Protocol_::getValueBigEndian16(bytes[7], bytes[8]);
    pos_y = Protocol_::getValueBigEndian16(bytes[9], bytes[10]);
}

std::vector<uint8_t> ItemInfo::toBytes() const {
    std::vector<uint8_t> buffer;

    // EntityID (4 bytes)
    Protocol_::insertBigEndian32(server_entt_id, buffer);
    // Type (1 byte)
    buffer.push_back(Protocol_::encodeTypeItem(type));
    // ammo (2bytes)
    Protocol_::insertBigEndian16(ammo, buffer);
    // pos (2 bytes cada uno)
    Protocol_::insertBigEndian16(pos_x, buffer);
    Protocol_::insertBigEndian16(pos_y, buffer);

    return buffer;
}

SpriteType ItemInfo::getSpriteType() {
    switch (type) {
        case TypeItem::Glock:
            return SpriteType::DROP_GLOCK;
        case TypeItem::Ak47:
            return SpriteType::DROP_AK47;
        case TypeItem::M3:
            return SpriteType::DROP_M3;
        case TypeItem::Awp:
            return SpriteType::DROP_AWP;
        case TypeItem::Bomb:
            return SpriteType::DROP_BOMB;
        default:
            throw std::runtime_error("Falla en ItemInfo::getSpriteType()");
    }
}
