#ifndef PLAYER_STATS_INFO_H
#define PLAYER_STATS_INFO_H

#include <cstdint>
#include <string>
#include <vector>

#include "../protocol.h"

struct PlayerStatsInfo {
    std::string username;
    int kills;
    int deaths;
    int moneyEarned;

    PlayerStatsInfo(const std::string& username, int kills, int deaths, int moneyEarned);
    explicit PlayerStatsInfo(const std::vector<uint8_t>& bytes);
    std::vector<uint8_t> toBytes() const;
};

#endif
