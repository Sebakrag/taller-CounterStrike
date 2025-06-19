#ifndef TYPES_SERVER_H_
#define TYPES_SERVER_H_

// capaz que este archivo no hace falta. Si no hay mas
// structs para definir, se puede definir en otro lado
#include <memory>
#include <utility>

#include "../../common/game_info/game_info.h"
#include "../../common/queue.h"
#include "weapon/weapon.h"

struct PlayerCommunicationChannels {
    std::shared_ptr<Queue<GameInfo>> senderQueue;
    std::shared_ptr<Queue<PlayerAction>> receiverQueue;
};

struct DroppedWeapon {
    std::unique_ptr<Weapon_> weapon;
    Vec2D position;

    DroppedWeapon(std::unique_ptr<Weapon_> w, const Vec2D pos):
            weapon(std::move(w)), position(pos) {}
    WeaponInfo generateWeaponInfo() const {
        WeaponInfo weaponInfo(weapon->generateWeaponInfo(WeaponState::DROPPED));
        weaponInfo.pos_x = position.getX();
        weaponInfo.pos_y = position.getY();
        return weaponInfo;
    }
};


#endif  // TYPES_SERVER_H_
