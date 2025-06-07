#include "../include/player.h"
#include <iostream>
#include <string>

Player::Player(const std::string& name, const Team team):
        name(name),
        team(team),
        posX(320),
        posY(200),
        health(100),
        state(PlayerState::Idle),
        knife(WeaponFactory::create(Weapon::Knife)),
        equippedWeapon(TypeWeapon::Knife),
        money(800),
        kills(0) {}


void Player::setPrimaryWeapon(std::unique_ptr<Weapon_> weapon) {
    primaryWeapon = std::move(weapon);
}

void Player::setEquippedWeapon(TypeWeapon type) { equippedWeapon = type; }

int Player::attack(float targetX, float targetY, uint64_t currentTimeMs) {
    if (state == PlayerState::Dead)
        return -1;

    std::cout << "Player " << name << " attacking target at (" << targetX << ", " << targetY
              << ")\n";
    switch (equippedWeapon) {
        case TypeWeapon::Knife:
            return knife ? knife->use(currentTimeMs) : -1;
        case TypeWeapon::Primary:
            return primaryWeapon ? primaryWeapon->use(currentTimeMs) : -1;
        case TypeWeapon::Secondary:
            return secondaryWeapon ? secondaryWeapon->use(currentTimeMs) : -1;
        default:
            return -1;
    }
}

float Player::getX() const { return posX; }
void Player::setX(const float x) { posX = x; }

float Player::getY() const { return posY; }
void Player::setY(const float y) { posY = y; }

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


Weapon_ *Player::getPrimaryWeapon() const {
    return primaryWeapon.get();
}

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
    if (!isAlive()) return;

    health -= dmg;
    if (health <= 0) {
        health = 0;
        state = PlayerState::Dead;
    }
}

int Player::shoot(uint64_t currentTimeMs) {
    if (!isAlive() || !canShoot(currentTimeMs))
        return 0;

    switch (equippedWeapon) {
        case TypeWeapon::Knife:
            return knife ? knife->use(currentTimeMs) : 0;
        case TypeWeapon::Primary:
            return primaryWeapon ? primaryWeapon->use(currentTimeMs) : 0;
        case TypeWeapon::Secondary:
            return secondaryWeapon ? secondaryWeapon->use(currentTimeMs) : 0;
        default:
            return 0;
    }
}


