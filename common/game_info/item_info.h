#ifndef ITEM_INFO_H_
#define ITEM_INFO_H_

#include <cstdint>
#include <vector>

#include "../protocol.h"
#include "../types.h"

#define SIZE_ITEM_INFO 11  // 7
struct ItemInfo {
    unsigned int server_entt_id;
    TypeItem type;
    int ammo = 0;  // si es un arma, tendrá sus balas
    int pos_x;
    int pos_y;

    ItemInfo() = default;
    ItemInfo(unsigned int server_entt_id, TypeItem type, int ammo, int pos_x, int pos_y);
    ItemInfo(const ItemInfo& other);

    ItemInfo& operator=(const ItemInfo& other);

    explicit ItemInfo(const std::vector<uint8_t>& bytes);

    std::vector<uint8_t> toBytes() const;

    SpriteType getSpriteType();
};

#endif  // ITEM_INFO_H_
