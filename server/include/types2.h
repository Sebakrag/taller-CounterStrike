#ifndef TYPES_SERVER_H_
#define TYPES_SERVER_H_

// capaz que este archivo no hace falta. Si no hay mas
// structs para definir, se puede definir en otro lado
#include <memory>

#include "../../common/game_info/game_info.h"
#include "../../common/queue.h"

struct PlayerCommunicationChannels {
    std::shared_ptr<Queue<GameInfo>> senderQueue;
    std::shared_ptr<Queue<PlayerAction>> receiverQueue;
};

struct DroppedWeapon {
    std::unique_ptr<Weapon_> weapon;
    Vec2D position;

    DroppedWeapon(std::unique_ptr<Weapon_> w, Vec2D pos) : weapon(std::move(w)), position(pos) {}
};


#endif  // TYPES_SERVER_H_
