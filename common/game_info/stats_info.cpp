#include "stats_info.h"

#include <iostream>

#include "../protocol.h"

StatsInfo::StatsInfo(const std::vector<uint8_t>& bytes) {
    size_t index = 0;
    uint16_t nT = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;
    for (int i = 0; i < nT; ++i) {
        uint16_t size = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
        index += 2;
        std::vector<uint8_t> slice(bytes.begin() + index, bytes.begin() + index + size);
        terroristStats.emplace_back(slice);
        index += size;
    }

    uint16_t nCT = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;
    for (int i = 0; i < nCT; ++i) {
        uint16_t size = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
        index += 2;
        std::vector<uint8_t> slice(bytes.begin() + index, bytes.begin() + index + size);
        counterStats.emplace_back(slice);
        index += size;
    }
}

std::vector<uint8_t> StatsInfo::toBytes() const {
    std::vector<uint8_t> bytes;

    Protocol_::insertBigEndian16(terroristStats.size(), bytes);
    for (const auto& ps: terroristStats) {
        auto p_bytes = ps.toBytes();
        Protocol_::insertBigEndian16(p_bytes.size(), bytes);
        bytes.insert(bytes.end(), p_bytes.begin(), p_bytes.end());
    }

    Protocol_::insertBigEndian16(counterStats.size(), bytes);
    for (const auto& ps: counterStats) {
        auto p_bytes = ps.toBytes();
        Protocol_::insertBigEndian16(p_bytes.size(), bytes);
        bytes.insert(bytes.end(), p_bytes.begin(), p_bytes.end());
    }

    return bytes;
}

void StatsInfo::print() const {
    std::cout << "== StatisticsInfo ==" << std::endl;
    std::cout << "Terrorists:" << std::endl;
    for (const auto& p: terroristStats)
        std::cout << p.username << ": K=" << p.kills << ", D=" << p.deaths << ", $" << p.moneyEarned
                  << std::endl;

    std::cout << "Counter-Terrorists:" << std::endl;
    for (const auto& p: counterStats)
        std::cout << p.username << ": K=" << p.kills << ", D=" << p.deaths << ", $" << p.moneyEarned
                  << std::endl;
}
