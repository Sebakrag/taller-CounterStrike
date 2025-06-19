#ifndef BOMB_INFO_H
#define BOMB_INFO_H

#include <vector>
#include <cstdint>

#include "../types.h"
#include "common/utils/Vec2D.h"

struct BombInfo {
    uint32_t serverId;
    BombState state;
    Vec2D position;

    BombInfo();
    BombInfo(uint32_t serverId, BombState state, const Vec2D& position);

    explicit BombInfo(const std::vector<uint8_t>& bytes);
    std::vector<uint8_t> toBytes() const;

    void print() const;
};

#endif
