#include "player.h"

Player::Player(const int id, const PlayerType type)
    : id(id), type(type), posX(0), posY(0), health(100),
      state(PlayerState::ALIVE), primaryWeapon(nullptr),
      secondaryWeapon(nullptr), equippedWeapon(WeaponType::KNIFE),
      money(800), kills(0) {}


void Player::setPosition(const int x, const int y) {
    posX = x;
    posY = y;
}

void Player::receiveDamage(const int dmg) {
    if (state == PlayerState::DEAD) return;

    health -= dmg;
    if (health <= 0) {
        health = 0;
        state = PlayerState::DEAD;
    }
}

void Player::setPrimaryWeapon(FireWeapon *weapon) {
    primaryWeapon = weapon;
}

void Player::setEquippedWeapon(WeaponType type) {
    equippedWeapon = type;
}

int Player::attack(int targetX, int targetY) {
    if (state == PlayerState::DEAD) return -1;

    switch (equippedWeapon) {
        case WeaponType::KNIFE:
            return knife.use();
        case WeaponType::PRIMARY:
            return primaryWeapon ? primaryWeapon->use() : -1;
        case WeaponType::SECONDARY:
            return secondaryWeapon ? secondaryWeapon->use() : -1;
        default:
            return -1;
    }
}

int Player::getX() const { return posX; }

int Player::getY() const { return posY; }

int Player::getId() const { return id; }

PlayerType Player::getType() const { return type; }

int Player::getHealth() const { return health; }

WeaponType Player::getEquippedWeapon() const { return equippedWeapon; }

FireWeapon* Player::getPrimaryWeapon() const { return primaryWeapon; }

FireWeapon* Player::getSecondaryWeapon() const { return secondaryWeapon; }

bool Player::isAlive() const { return state == PlayerState::ALIVE; }






