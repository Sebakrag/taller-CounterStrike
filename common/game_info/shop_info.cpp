#include "shop_info.h"

#include <iostream>

#include "../protocol.h"

ShopInfo::ShopInfo() {}

ShopInfo::ShopInfo(const std::unordered_map<Weapon, int>& weaponPrices,
                   const std::unordered_map<AmmoType, int>& ammoPrices):
        weaponPrices(weaponPrices), ammoPrices(ammoPrices) {}

std::vector<uint8_t> ShopInfo::toBytes() const {
    std::vector<uint8_t> buffer;

    Protocol_::insertBigEndian16(weaponPrices.size(), buffer);
    for (const auto& [weapon, price]: weaponPrices) {
        buffer.push_back(Protocol_::encodeWeapon(weapon));
        Protocol_::insertBigEndian16(price, buffer);
    }

    Protocol_::insertBigEndian16(ammoPrices.size(), buffer);
    for (const auto& [ammoType, price]: ammoPrices) {
        buffer.push_back(Protocol_::encodeAmmoType(ammoType));
        Protocol_::insertBigEndian16(price, buffer);
    }

    return buffer;
}

ShopInfo::ShopInfo(const std::vector<uint8_t>& bytes) {
    size_t index = 0;

    uint16_t weaponCount = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;
    for (uint16_t i = 0; i < weaponCount; ++i) {
        Weapon w = Protocol_::decodeWeapon(bytes[index++]);
        int price = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
        index += 2;
        weaponPrices[w] = price;
    }

    uint16_t ammoCount = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;
    for (uint16_t i = 0; i < ammoCount; ++i) {
        AmmoType w = Protocol_::decodeAmmoType(bytes[index++]);
        int price = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
        index += 2;
        ammoPrices[w] = price;
    }
}

void ShopInfo::print() const {
    std::cout << "=== Shop Info ===" << std::endl;

    std::cout << "Weapon Prices:" << std::endl;
    for (const auto& [w, price]: weaponPrices) {
        std::cout << "  Weapon: " << static_cast<int>(w) << " -> $" << price << std::endl;
    }

    std::cout << "Ammo Prices:" << std::endl;
    for (const auto& [a, price]: ammoPrices) {
        std::cout << "  Weapon: " << static_cast<int>(a) << " -> $" << price << std::endl;
    }

    std::cout << "=================" << std::endl;
}