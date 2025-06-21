#ifndef SHOP_H
#define SHOP_H

#include "player.h"
#include "weapon/weapon_factory.h"
#include "match.h"
#include "common/game_info/shop_info.h"
#include "types2.h"

#include <unordered_map>


class Shop {
public:
    static bool buyPrimaryWeapon(Player& player, Weapon weaponToBuy, std::vector<DroppedWeapon>& droppedWeapons);
    static bool buyAmmo(Player& player, Weapon selectedWeapon, int amount);
    static ShopInfo getInfo();
};

#endif