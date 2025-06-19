#include "../include/shop.h"

namespace { //Precios de armas y municiones
    const std::unordered_map<Weapon, int> weaponPrices = {
        {Weapon::Ak47, 200},
        {Weapon::M3, 300},
        {Weapon::Awp, 450},
    };

    const std::unordered_map<Weapon, int> ammoPrices = {
        {Weapon::Ak47, 20},
        {Weapon::M3, 30},
        {Weapon::Awp, 50},
    };
}

bool Shop::buyPrimaryWeapon(Player &player, Weapon weaponToBuy, std::vector<DroppedWeapon> &droppedWeapons) {
    if (!weaponPrices.contains(weaponToBuy))
        return false;

    int price = weaponPrices.at(weaponToBuy);
    if (!player.spendMoney(price))
        return false;

    //Drop del arma anterior
    if (player.getPrimaryWeapon()) {
        if (player.getPrimaryWeapon()) {
            droppedWeapons.emplace_back(
                std::move(player.dropPrimaryWeapon()),
                Vec2D(player.getX(), player.getY())
                );
        }
    }

    player.setPrimaryWeapon(WeaponFactory::create(weaponToBuy));
    return true;
}

bool Shop::buyAmmo(Player& player, Weapon selectedWeapon, int amount) {
    if (!ammoPrices.contains(selectedWeapon))
        return false;

    Weapon_* current = player.getPrimaryWeapon();
    if (!current || current->getWeaponType() != selectedWeapon)
        return false;

    auto* fireWeapon = dynamic_cast<FireWeapon*>(current);
    if (!fireWeapon) {
        std::cout << "Esta arma no admite munición\n";
        return false;
    }

    int price = ammoPrices.at(selectedWeapon) * amount;
    if (!player.spendMoney(price))
        return false;

    fireWeapon->addBullets(amount);
    return true;
}

ShopInfo Shop::getInfo() {
    return ShopInfo(weaponPrices, ammoPrices);
}
