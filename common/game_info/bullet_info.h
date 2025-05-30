#ifndef BULLET_INFO_H_
#define BULLET_INFO_H_

#include <cstdint>
#include <vector>

#include "../protocol.h"
#include "../types.h"

#define SIZE_BULLET_INFO 13

struct BulletInfo {
    int id;
    Weapon weapon;  // para diferenciar tipo de bala
    int pos_x;
    int pos_y;
    Vector2 direction;

    BulletInfo() = default;
    BulletInfo(int id, Weapon weapon, int pos_x, int pos_y, Vector2 direction);
    BulletInfo(const BulletInfo& other);

    BulletInfo& operator=(const BulletInfo& other);

    explicit BulletInfo(const std::vector<uint8_t>& bytes);

    std::vector<uint8_t> toBytes() const;
};
#endif  // BULLET_INFO_H_
