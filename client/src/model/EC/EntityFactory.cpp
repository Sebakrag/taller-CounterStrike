#include "client/include/model/EC/EntityFactory.h"

#include "client/include/model/EC/components/PositionComponent.h"
#include "client/include/model/EC/components/SpriteComponent.h"


EntityFactory::EntityFactory(ComponentManager& cm): comp_mgr(cm) {}

void EntityFactory::create_specific_entity(const Entity& new_entt,
                                           const EntitySnapshot& snap) const {
    switch (snap.entt_type) {
        // case EntityType::TERRORIST: {
        //     create_terrorist_entt();
        //     break;
        // }
        case EntityType::ANTI_TERRORIST: {
            create_anti_terrorist_entt(new_entt, snap);
            break;
        }
            // case EntityType::WEAPON: {
            //     create_weapon_entt();
            //     break;
            // }
            // case EntityType::BULLET: {
            //     create_bullet_entt();
            //     break;
            // }
            // case EntityType::BOMB: {
            //     create_bomb_entt();
            //     break;
            // }
        default:
            break;
    }
}

void EntityFactory::create_anti_terrorist_entt(const Entity& new_entt,
                                               const EntitySnapshot& snap) const {
    const auto pos_comp = comp_mgr.addComponent<PositionComponent>(new_entt);
    pos_comp->init(snap.pos_x, snap.pos_y);

    const auto sprite_comp = comp_mgr.addComponent<SpriteComponent>(new_entt);
    sprite_comp->init(snap.sprite_type, snap.pos_x, snap.pos_y);
}
