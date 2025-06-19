#ifndef SHOP_INFO_H
#define SHOP_INFO_H

#include <cstdint>
#include <unordered_map>
#include <vector>
#include "../types.h"

struct ShopInfo {
    std::unordered_map<Weapon, int> weaponPrices;
    std::unordered_map<Weapon, int> ammoPrices;

    ShopInfo();
    ShopInfo(const std::unordered_map<Weapon, int>& weaponPrices,
             const std::unordered_map<Weapon, int>& ammoPrices);

    explicit ShopInfo(const std::vector<uint8_t>& bytes);
    std::vector<uint8_t> toBytes() const;

    void print() const;
};

#endif
