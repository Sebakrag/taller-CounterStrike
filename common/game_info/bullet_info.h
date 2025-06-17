#ifndef BULLET_INFO_H_
#define BULLET_INFO_H_

#include <cstdint>
#include <vector>

#include "..//types.h"
#include "../protocol.h"

#define SIZE_BULLET_INFO 15  // 13

struct BulletInfo {
    ServerEntityID id;  // server_entt_id
    Weapon weapon;  // para diferenciar tipo de bala //TODO: Definamos un BulletType. Puede haber
                    // balas que se compartan entre diferentes armas.
    int pos_x;
    int pos_y;
    Vec2D direction;

    BulletInfo() = default;
    BulletInfo(ServerEntityID id, Weapon weapon, int pos_x, int pos_y, const Vec2D& direction);

    BulletInfo(const BulletInfo& other) = default;
    BulletInfo& operator=(const BulletInfo& other) = default;

    explicit BulletInfo(const std::vector<uint8_t>& bytes);

    std::vector<uint8_t> toBytes() const;

    SpriteType getSpriteType();
};
#endif  // BULLET_INFO_H_
