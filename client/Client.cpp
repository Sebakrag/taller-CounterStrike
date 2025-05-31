#include "Client.h"

Client::Client(const EntitySnapshot& snap):
        snap(snap), x(snap.pos_x), y(snap.pos_y), angle(snap.angle) {}


std::vector<EntitySnapshot> Client::getGameSnapshot() const {
    const EntitySnapshot s = {snap.server_entt_id, this->x,        this->y, this->angle,
                              snap.sprite_type,    snap.entt_type, snap.hp, snap.money,
                              snap.is_alive};
    std::vector<EntitySnapshot> v(1, s);

    return v;
}

void Client::move(const Vec2D& direction) {
    this->x += direction.getX();
    this->y += direction.getY();
}

void Client::shoot(const AimInfo& aimInfo) { this->angle = aimInfo.angle; }

void Client::rotate(const float angle) { this->angle = angle; }
