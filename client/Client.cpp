#include "Client.h"

Client::Client(const EntitySnapshot& snap): snap(snap), x(snap.pos_x), y(snap.pos_y) {}


std::vector<EntitySnapshot> Client::getGameSnapshot() const {
    const EntitySnapshot s = {
            snap.server_entt_id, x,       y,          snap.angle,   snap.sprite_type,
            snap.entt_type,      snap.hp, snap.money, snap.is_alive};
    std::vector<EntitySnapshot> v(1, s);

    return v;
}

void Client::move(const Vec2D& direction) {
    x += direction.getX();
    y += direction.getY();
}
