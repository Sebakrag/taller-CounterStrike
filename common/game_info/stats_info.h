#ifndef STATS_INFO_H
#define STATS_INFO_H

#include "player_stats_info.h"

struct StatsInfo {
    std::vector<PlayerStatsInfo> terroristStats;
    std::vector<PlayerStatsInfo> counterStats;

    StatsInfo() = default;
    explicit StatsInfo(const std::vector<uint8_t>& bytes);
    std::vector<uint8_t> toBytes() const;

    void print() const;
};

#endif