#include "item_info.h"

ItemInfo::ItemInfo(TypeItem type, int ammo, int pos_x, int pos_y):
        type(type), ammo(ammo), pos_x(pos_x), pos_y(pos_y) {}

ItemInfo::ItemInfo(const ItemInfo& other):
        type(other.type), ammo(other.ammo), pos_x(other.pos_x), pos_y(other.pos_y) {}

ItemInfo& ItemInfo::operator=(const ItemInfo& other) {
    if (this != &other) {
        type = other.type;
        ammo = other.ammo;
        pos_x = other.pos_x;
        pos_y = other.pos_y;
    }
    return *this;
}

ItemInfo::ItemInfo(const std::vector<uint8_t>& bytes) {
    // Type (1 byte)
    type = Protocol_::decodeTypeItem(bytes[0]);

    // ammo (2 bytes)
    ammo = Protocol_::getValueBigEndian16(bytes[1], bytes[2]);

    // pos (2 bytes cada uno)
    pos_x = Protocol_::getValueBigEndian16(bytes[3], bytes[4]);
    pos_y = Protocol_::getValueBigEndian16(bytes[5], bytes[6]);
}

std::vector<uint8_t> ItemInfo::toBytes() const {
    std::vector<uint8_t> buffer;
    // Type (1 byte)
    buffer.push_back(Protocol_::encodeTypeItem(type));
    // ammo (2bytes)
    Protocol_::insertBigEndian16(ammo, buffer);
    // pos (2 bytes cada uno)
    Protocol_::insertBigEndian16(pos_x, buffer);
    Protocol_::insertBigEndian16(pos_y, buffer);
    return buffer;
}
