#include "../include/player.h"

#include <cmath>
#include <iostream>
#include <string>

#include "../../server/include/id_generator.h"

Player::Player(const std::string& name, const Team team):
        serverId(IdGenerator::getNextId()),
        name(name),
        team(team),
        posX(320),
        posY(200),
        health(100),
        state(PlayerState::Idle),
        knife(WeaponFactory::create(Weapon::Knife)),
        primaryWeapon(WeaponFactory::create(
                Weapon::Ak47)),  // esto hay que quitarlo cuando se pueda comprar armas.
        secondaryWeapon(WeaponFactory::create(Weapon::Glock)),
        equippedWeapon(TypeWeapon::Knife),
        money(800),
        kills(0),
        skinT(PlayerSkin::Terrorist3),
        skinCT(PlayerSkin::CounterTerrorist3) {
    std::cout << "Server ID del Player: " << serverId << std::endl;
}


void Player::setPrimaryWeapon(std::unique_ptr<Weapon_> weapon) {
    primaryWeapon = std::move(weapon);
}

void Player::setEquippedWeapon(TypeWeapon type) { equippedWeapon = type; }

float Player::getX() const { return posX; }
void Player::setX(const float x) { posX = x; }

float Player::getY() const { return posY; }
void Player::setY(const float y) { posY = y; }

float Player::getAngle() const { return angle; }
void Player::setAngle(float _angle) { angle = _angle; }

std::string Player::getId() const { return name; }

Team Player::getTeam() const { return team; }

int Player::getHealth() const { return health; }

float Player::getMoney() const { return money; }

float Player::getSpeed() const { return speed; }

TypeWeapon Player::getEquippedWeapon() const { return equippedWeapon; }

Weapon Player::getSpecificEquippedWeapon() const {
    switch (equippedWeapon) {
        case TypeWeapon::Knife:
            return knife ? knife->getWeaponType() : Weapon::None;
        case TypeWeapon::Primary:
            return primaryWeapon ? primaryWeapon->getWeaponType() : Weapon::None;
        case TypeWeapon::Secondary:
            return secondaryWeapon ? secondaryWeapon->getWeaponType() : Weapon::None;
        default:
            return Weapon::None;
    }
}

Weapon_* Player::getEquippedWeaponInstance() {
    switch (equippedWeapon) {
        case TypeWeapon::Knife:
            return knife.get();
        case TypeWeapon::Primary:
            return primaryWeapon.get();
        case TypeWeapon::Secondary:
            return secondaryWeapon.get();
        default:
            return nullptr;
    }
}

Weapon_* Player::getPrimaryWeapon() const { return primaryWeapon.get(); }

bool Player::isAlive() const { return state != PlayerState::Dead; }

bool Player::canShoot(uint64_t currentTimeMs) const {
    switch (equippedWeapon) {
        case TypeWeapon::Knife:
            return knife && knife->canShoot(currentTimeMs);
        case TypeWeapon::Primary:
            return primaryWeapon && primaryWeapon->canShoot(currentTimeMs);
        case TypeWeapon::Secondary:
            return secondaryWeapon && secondaryWeapon->canShoot(currentTimeMs);
        default:
            return false;
    }
}

void Player::takeDamage(int dmg) {
    if (!isAlive())
        return;

    health -= dmg;
    if (health <= 0) {
        health = 0;
        state = PlayerState::Dead;
    }
}

std::vector<Projectile> Player::shoot(float dirX, float dirY, uint64_t currentTimeMs) {
    Weapon_* weapon = getEquippedWeaponInstance();
    if (!weapon || !weapon->canShoot(currentTimeMs))
        return {};

    return weapon->shoot(posX, posY, dirX, dirY, name, currentTimeMs);
}

std::unique_ptr<Weapon_> Player::dropPrimaryWeapon() { return std::move(primaryWeapon); }

uint32_t Player::getServerId() const { return serverId; }

LocalPlayerInfo Player::generateLocalPlayerInfo() const {
    PlayerSkin currentSkin = (team == Team::CounterTerrorist) ? skinCT : skinT;
    int ammo = 0;
    if (equippedWeapon == TypeWeapon::Primary) {
        ammo = primaryWeapon->getBullets();
    } else if (equippedWeapon == TypeWeapon::Secondary) {
        ammo = secondaryWeapon->getBullets();
    }
    Weapon currentWeapon = Weapon::None;
    if (equippedWeapon == TypeWeapon::Primary) {
        currentWeapon = primaryWeapon->getWeaponType();
    } else if (equippedWeapon == TypeWeapon::Secondary) {
        currentWeapon = secondaryWeapon->getWeaponType();
    } else if (equippedWeapon == TypeWeapon::Knife) {
        currentWeapon = Weapon::Knife;
    } else if (equippedWeapon == TypeWeapon::Bomb) {
        currentWeapon = Weapon::Bomb;
    }
    return LocalPlayerInfo(serverId, team, currentSkin, Vec2D(posX, posY), angle, currentWeapon,
                           health, money, ammo);
}

PlayerInfo Player::generatePlayerInfo() const {
    PlayerSkin currentSkin = (team == Team::CounterTerrorist) ? skinCT : skinT;
    Weapon currentWeapon = Weapon::None;
    if (equippedWeapon == TypeWeapon::Primary) {
        currentWeapon = primaryWeapon->getWeaponType();
    } else if (equippedWeapon == TypeWeapon::Secondary) {
        currentWeapon = secondaryWeapon->getWeaponType();
    } else if (equippedWeapon == TypeWeapon::Knife) {
        currentWeapon = Weapon::Knife;
    } else if (equippedWeapon == TypeWeapon::Bomb) {
        currentWeapon = Weapon::Bomb;
    }

    return PlayerInfo(serverId, name, team, currentSkin, Vec2D(posX, posY), angle, currentWeapon);
}
