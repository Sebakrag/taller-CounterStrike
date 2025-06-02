#ifndef BULLET_INFO_H_
#define BULLET_INFO_H_

#include <cstdint>
#include <vector>

#include "..//types.h"
#include "../protocol.h"

#define SIZE_BULLET_INFO 15  // 13

struct BulletInfo {
    unsigned int id;  // server_entt_id
    Weapon weapon;    // para diferenciar tipo de bala
    int pos_x;
    int pos_y;
    Vec2D direction;

    BulletInfo() = default;
    BulletInfo(unsigned int id, Weapon weapon, int pos_x, int pos_y, const Vec2D& direction);
    BulletInfo(const BulletInfo& other);

    BulletInfo& operator=(const BulletInfo& other);

    explicit BulletInfo(const std::vector<uint8_t>& bytes);

    std::vector<uint8_t> toBytes() const;

    SpriteType getSpriteType();
};
#endif  // BULLET_INFO_H_
