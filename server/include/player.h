#ifndef PLAYER_H
#define PLAYER_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "../../common/game_info/local_player_info.h"
#include "../../common/game_info/player_info.h"
#include "../../common/types.h"
#include "weapon/weapon.h"
#include "weapon/weapon_factory.h"

#include "id_generator.h"

class Player {
private:
    uint32_t serverId;
    std::string name;  // id
    Team team;
    float posX, posY;
    int health;
    PlayerState state;
    float speed = 80.0f;
    float angle = 0;

    std::unique_ptr<Weapon_> knife;
    std::unique_ptr<Weapon_> primaryWeapon;
    std::unique_ptr<Weapon_> secondaryWeapon;
    TypeWeapon equippedWeapon;
    uint32_t id_weapon;  // necesito esto

    float money;
    int kills;
    const PlayerSkin skinT;   // para cuando es terrorista
    const PlayerSkin skinCT;  // para cuando es anti terrorista

public:
    // TODO: Recibir las 2 skin en el constructor
    explicit Player(const std::string& name, const Team playerTeam);

    void setPrimaryWeapon(std::unique_ptr<Weapon_> weapon);
    void setEquippedWeapon(TypeWeapon type);

    float getX() const;
    void setX(const float x);
    float getY() const;
    void setY(const float y);
    float getAngle() const;
    void setAngle(float angle);
    std::string getId() const;
    Team getTeam() const;
    int getHealth() const;
    float getMoney() const;
    float getSpeed() const;
    TypeWeapon getEquippedWeapon() const;
    Weapon getSpecificEquippedWeapon() const;
    Weapon_* getPrimaryWeapon() const;
    bool isAlive() const;
    bool canShoot(uint64_t currentTimeMs) const;
    void takeDamage(int dmg);
    std::vector<Projectile> shoot(float dirX, float dirY, uint64_t currentTimeMs);
    Weapon_* getEquippedWeaponInstance() const;
    std::unique_ptr<Weapon_> dropPrimaryWeapon();
    void revive();
    void setTeam(Team newTeam);

    uint32_t getServerId() const;

    LocalPlayerInfo generateLocalPlayerInfo() const;
    PlayerInfo generatePlayerInfo() const;
};


#endif
