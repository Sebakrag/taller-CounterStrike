#ifndef BULLET_INFO_H_
#define BULLET_INFO_H_

#include <cstdint>
#include <vector>

#include "..//types.h"
#include "../protocol.h"

#define SIZE_BULLET_INFO 20

struct BulletInfo {
    ServerEntityID id;  // server_entt_id
    Weapon weapon;  // para diferenciar tipo de bala //TODO: Definamos un BulletType. Puede haber
                    // balas que se compartan entre diferentes armas.
    float pos_x;
    float pos_y;
    Vec2D direction;
    bool active;

    BulletInfo() = default;
    BulletInfo(ServerEntityID id, Weapon weapon, float pos_x, float pos_y, const Vec2D& direction, bool active = true);

    BulletInfo(const BulletInfo& other) = default;
    BulletInfo& operator=(const BulletInfo& other) = default;

    explicit BulletInfo(const std::vector<uint8_t>& bytes);

    std::vector<uint8_t> toBytes() const;
    void print() const;

    SpriteType getSpriteType();
};
#endif  // BULLET_INFO_H_
