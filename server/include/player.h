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
    float speed;
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
    explicit Player(const std::string& name, const Team playerTeam,
                    const Vec2D& position = Vec2D());

    void setPrimaryWeapon(std::unique_ptr<Weapon_> weapon);
    void setEquippedWeapon(TypeWeapon type);

    float getX() const;
    void setX(const float x);
    float getY() const;
    void setY(const float y);
    void setPostion(const Vec2D& pos);
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
    bool canShoot(double currentTime) const;
    void takeDamage(int dmg);
    std::vector<Projectile> shoot(float dirX, float dirY, double currentTime);
    Weapon_* getEquippedWeaponInstance() const;
    std::unique_ptr<Weapon_> dropPrimaryWeapon();
    void revive();
    void setTeam(Team newTeam);
    void setState(PlayerState newState);
    bool spendMoney(int amount);
    void addMoney(int amount);

    uint32_t getServerId() const;

    LocalPlayerInfo generateLocalPlayerInfo() const;
    PlayerInfo generatePlayerInfo() const;

    // CONSTANTES static para inicializar en server.cpp.
private:
    static bool initialized;     // para asegurar que se llame init una unica vez.
    static float PLAYER_SPEED;   // Velocidad de movimiento del jugador
    static float INITIAL_MONEY;  // Monedas iniciales para un jugador
    static int INITIAL_HEALTH;   // Vida inicial para un jugador

public:
    // Para cargar los valores del config.yaml
    static void init(float player_speed, float initial_money, int initial_health);
};


#endif
