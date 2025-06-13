#include "client/include/model/EC/EntityFactory.h"
#include "client/include/model/EC/components/RenderComponent.h"
#include "client/include/model/EC/components/SpriteComponent.h"
#include "client/include/model/EC/components/TransformComponent.h"
#include "common/dtos/EntitySnapshot.h"
#include "common/types.h"

#include <variant>
#include <utility>


EntityFactory::EntityFactory(ComponentManager& cm): comp_mgr(cm) {}

void EntityFactory::create_specific_entity(const Entity& new_entt,
                                           const EntitySnapshot& snap) const {
    // Usar el campo 'type' que existe en EntitySnapshot
    switch (snap.type) {
        case EntityType::PLAYER: {
            // Determinar si es terrorista o anti-terrorista basado en otros datos
            // Como ejemplo, usamos PlayerSnapshot si está disponible
            if (std::holds_alternative<PlayerSnapshot>(snap.data)) {
                const auto& player_data = std::get<PlayerSnapshot>(snap.data);
                if (player_data.team == Team::CounterTerrorist) {
                    create_anti_terrorist_entt(new_entt, snap);
                } else {
                    // Manejar terroristas si es necesario
                    // create_terrorist_entt(new_entt, snap);
                }
            } else {
                // Si no hay datos específicos, tratar como anti-terrorista por defecto
                create_anti_terrorist_entt(new_entt, snap);
            }
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
    const auto t_comp = comp_mgr.addComponent<TransformComponent>(new_entt);
    t_comp->init(snap.pos_x, snap.pos_y, snap.angle);

    const auto sprite_comp = comp_mgr.addComponent<SpriteComponent>(new_entt);
    sprite_comp->init(snap.sprite_type);

    comp_mgr.addComponent<RenderComponent>(new_entt);
}
