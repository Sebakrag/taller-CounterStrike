#include "../include/shop.h"

namespace {  // Precios de armas y municiones
const std::unordered_map<Weapon, int> weaponPrices = {
        {Weapon::Ak47, 200},
        {Weapon::M3, 300},
        {Weapon::Awp, 450},
};

const std::unordered_map<Weapon, int> bulletsPerCartridge = {
    {Weapon::Ak47, 15},
    {Weapon::M3, 10},
    {Weapon::Awp, 5},
    {Weapon::Glock, 10},
};

const std::unordered_map<AmmoType, int> ammoPrices = {
        {AmmoType::Primary, 20},
        {AmmoType::Secondary, 20},
};
}  // namespace

bool Shop::buyPrimaryWeapon(Player& player, Weapon weaponToBuy,
                            std::list<DroppedWeapon>& droppedWeapons) {
    if (!weaponPrices.contains(weaponToBuy))
        return false;

    int price = weaponPrices.at(weaponToBuy);
    if (!player.spendMoney(price))
        return false;

    // Drop del arma anterior
    if (player.getPrimaryWeapon()) {
        if (player.getPrimaryWeapon()) {
            droppedWeapons.emplace_back(std::move(player.dropPrimaryWeapon()),
                                        Vec2D(player.getX(), player.getY()));
        }
    }

    player.setPrimaryWeapon(WeaponFactory::create(weaponToBuy));
    return true;
}

bool Shop::buyAmmo(Player& player, AmmoType selectedAmmo, int /*amount*/) {
    if (!ammoPrices.contains(selectedAmmo))
        return false;

    Weapon_* weapon;
    switch (selectedAmmo) {
        case AmmoType::Primary: {
            weapon = player.getPrimaryWeapon();
            if (!weapon)
                return false;
            break;
        }
        case AmmoType::Secondary: {
            weapon = player.getSecondaryWeapon();
            if (!weapon)
                return false;
            break;
        }
        default:
            return false;
    }

    auto* fireWeapon = dynamic_cast<FireWeapon*>(weapon);
    if (!fireWeapon) {
        std::cout << "Esta arma no admite munición\n";
        return false;
    }

    Weapon weaponType = fireWeapon->getWeaponType();
    if (!bulletsPerCartridge.contains(weaponType))
        return false;
    int bulletsToAdd = bulletsPerCartridge.at(weaponType);
    int price = ammoPrices.at(selectedAmmo);
    if (!player.spendMoney(price))
        return false;

    fireWeapon->addBullets(bulletsToAdd);
    return true;
}

ShopInfo Shop::getInfo() { return ShopInfo(weaponPrices, ammoPrices); }