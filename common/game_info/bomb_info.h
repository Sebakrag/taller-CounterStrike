#ifndef BOMB_INFO_H
#define BOMB_INFO_H

#include <cstdint>
#include <vector>

#include "../protocol.h"
#include "../types.h"

#define SIZE_BOMB_INFO 13

struct BombInfo {
    ServerEntityID server_entt_id;
    BombState state;
    float pos_x;
    float pos_y;

    BombInfo() = default;
    BombInfo(ServerEntityID server_entt_id, BombState state, float pos_x, float pos_y);

    BombInfo(const BombInfo& other) = default;
    BombInfo& operator=(const BombInfo& other) = default;

    explicit BombInfo(const std::vector<uint8_t>& bytes);

    std::vector<uint8_t> toBytes() const;

    SpriteType getSpriteType() const;
};

#endif  // BOMB_INFO_H
