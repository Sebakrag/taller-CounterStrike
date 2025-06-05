#include "client/include/model/EC/EntityFactory.h"

#include "client/include/model/EC/components/EquippedWeaponComponent.h"
#include "client/include/model/EC/components/PlayerSpriteComponent.h"
#include "client/include/model/EC/components/TransformComponent.h"
#include "client/include/model/EC/components/WeaponSpriteComponent.h"


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
        case EntityType::WEAPON: {
            create_weapon_entt(new_entt, snap);
            break;
        }
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
    const auto tComp = comp_mgr.addComponent<TransformComponent>(new_entt);
    tComp->init(snap.pos_x, snap.pos_y, snap.angle);

    const auto spriteComp = comp_mgr.addComponent<PlayerSpriteComponent>(new_entt);
    spriteComp->init(snap.sprite_type);

    const auto equippedWeapon = comp_mgr.addComponent<EquippedWeaponComponent>(new_entt);
    equippedWeapon->setID(INVALID_ENTITY);
}

void EntityFactory::create_weapon_entt(const Entity& new_entt, const EntitySnapshot& snap) const {
    const auto tComp = comp_mgr.addComponent<TransformComponent>(new_entt);
    tComp->init(snap.pos_x, snap.pos_y, snap.angle);

    const auto spriteComp = comp_mgr.addComponent<WeaponSpriteComponent>(new_entt);
    std::cout << "[DEBUG] inicializo weaponSprite con el estado: "
              << static_cast<int>(snap.weapon_state) << std::endl;
    spriteComp->init(snap.sprite_type, snap.weapon_state);
    std::cout << "Cree el arma con el serverID: " << snap.server_entt_id << "\n"
              << "Tiene el estado: " << static_cast<int>(snap.weapon_state) << std::endl;
}
