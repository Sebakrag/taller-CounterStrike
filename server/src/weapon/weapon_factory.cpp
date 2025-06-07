#include "../../include/weapon/weapon_factory.h"

std::unique_ptr<Weapon_> WeaponFactory::create(Weapon type) {
    switch (type) {
        case Weapon::Knife:
            return std::make_unique<WeaponKnife>();
        case Weapon::Glock:
            return std::make_unique<WeaponGlock>();
        case Weapon::Ak47:
            return std::make_unique<WeaponAk47>();
        case Weapon::Awp:
            return std::make_unique<WeaponAwp>();
        case Weapon::M3:
            return std::make_unique<WeaponM3>();
        default:
            throw std::invalid_argument("Unknown weapon type");
    }
}