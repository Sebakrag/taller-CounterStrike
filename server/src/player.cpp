#include "../include/player.h"

#include <iostream>
#include <string>

Player::Player(const std::string& name, const Team team):
        name(name),
        team(team),
        posX(0),
        posY(0),
        health(100),
        state(PlayerState::Idle),
        primaryWeapon(nullptr),
        secondaryWeapon(nullptr),
        equippedWeapon(TypeWeapon::Knife),
        money(800),
        kills(0) {}


void Player::receiveDamage(const int dmg) {
    if (state == PlayerState::Dead)
        return;

    health -= dmg;
    if (health <= 0) {
        health = 0;
        state = PlayerState::Dead;
    }
}

void Player::setPrimaryWeapon(FireWeapon* weapon) { primaryWeapon = weapon; }

void Player::setEquippedWeapon(TypeWeapon type) { equippedWeapon = type; }

int Player::attack(float targetX, float targetY) {
    if (state == PlayerState::Dead)
        return -1;

    std::cout << "Player " << name << " attacking target at (" << targetX << ", " << targetY
              << ")\n";
    switch (equippedWeapon) {
        case TypeWeapon::Knife:
            return knife.use();
        case TypeWeapon::Primary:
            return primaryWeapon ? primaryWeapon->use() : -1;
        case TypeWeapon::Secondary:
            return secondaryWeapon ? secondaryWeapon->use() : -1;
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

FireWeapon* Player::getPrimaryWeapon() const { return primaryWeapon; }

FireWeapon* Player::getSecondaryWeapon() const { return secondaryWeapon; }

bool Player::isAlive() const { return state != PlayerState::Dead; }
