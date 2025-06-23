#ifndef SHOP_H
#define SHOP_H

#include "weapon/weapon_factory.h"

#include "player.h"
//#include "match.h"
#include <list>
#include <unordered_map>

#include "../../common/game_info/shop_info.h"
#include "../../server/include/types2.h"

#include "types2.h"

class Shop {
public:
    static bool buyPrimaryWeapon(Player& player, Weapon weaponToBuy,
                                 std::list<DroppedWeapon>& droppedWeapons);
    static bool buyAmmo(Player& player, Weapon selectedWeapon, int amount);
    static ShopInfo getInfo();
};

#endif
