#include "../../../../client/include/model/EC/EntityFactory.h"

#include "../../../../client/include/model/EC/components/EquippedWeaponComponent.h"
#include "../../../../client/include/model/EC/components/PlayerSpriteComponent.h"
#include "../../../../client/include/model/EC/components/TransformComponent.h"
#include "../../../../client/include/model/EC/components/WeaponSpriteComponent.h"
#include "model/EC/components/BulletSpriteComponent.h"


EntityFactory::EntityFactory(ComponentManager& cm, const int numPlayers): comp_mgr(cm) {
    setPlayersComponentsCapacity(numPlayers);
}

void EntityFactory::setPlayersComponentsCapacity(const int numPlayers) const {
    comp_mgr.setCapacity<PlayerSpriteComponent>(numPlayers);
    comp_mgr.setCapacity<EquippedWeaponComponent>(numPlayers);
}

void EntityFactory::create_specific_entity(const Entity& new_entt,
                                           const EntitySnapshot& snap) const {
    switch (snap.type) {
        case EntityType::PLAYER: {
            create_player_entt(new_entt, snap);
            break;
        }
        case EntityType::WEAPON: {
            create_weapon_entt(new_entt, snap);
            break;
        }
        case EntityType::BULLET: {
            create_bullet_entt(new_entt, snap);
            break;
        }
        // case EntityType::BOMB: { // TODO: me parece que me conviene tratar a la bomba como un
        // arma mas.
        //     create_bomb_entt();
        //     break;
        // }
        default:
            break;
    }
}

void EntityFactory::createEntityPlayer(const Entity& new_entt, const LocalPlayerInfo& p) {
    const auto tComp = comp_mgr.addComponent<TransformComponent>(new_entt);
    tComp->init(p.position.getX(), p.position.getY(), p.angle_direction);

    const auto spriteComp = comp_mgr.addComponent<PlayerSpriteComponent>(new_entt);
    spriteComp->init(p.generateSpriteType(), p.weapon_type);

    const auto equippedWeapon = comp_mgr.addComponent<EquippedWeaponComponent>(new_entt);
    equippedWeapon->setID(
            INVALID_ENTITY);  // TODO: Si lo dejamos asi, todo jugador empieza desarmado.
}

void EntityFactory::create_player_entt(const Entity& new_entt, const EntitySnapshot& snap) const {
    if (const auto player = std::get_if<PlayerSnapshot>(&snap.data)) {
        const auto tComp = comp_mgr.addComponent<TransformComponent>(new_entt);
        tComp->init(snap.pos_x, snap.pos_y, snap.angle);

        const auto spriteComp = comp_mgr.addComponent<PlayerSpriteComponent>(new_entt);
        spriteComp->init(snap.sprite_type, player->weapon_type);

        const auto equippedWeapon = comp_mgr.addComponent<EquippedWeaponComponent>(new_entt);
        equippedWeapon->setID(
                INVALID_ENTITY);  // TODO: Si lo dejamos asi, todo jugador empieza desarmado.
    } else {
        throw std::runtime_error("Error trying to create a Player entity.");
    }
}

void EntityFactory::create_weapon_entt(const Entity& new_entt, const EntitySnapshot& snap) const {
    if (const auto weapon = std::get_if<WeaponSnapshot>(&snap.data)) {
        const auto tComp = comp_mgr.addComponent<TransformComponent>(new_entt);
        tComp->init(snap.pos_x, snap.pos_y, snap.angle);

        const auto spriteComp = comp_mgr.addComponent<WeaponSpriteComponent>(new_entt);
        spriteComp->init(snap.sprite_type, weapon->state);
    } else {
        throw std::runtime_error("Error trying to create a Weapon entity.");
    }
}

void EntityFactory::create_bullet_entt(const Entity& new_entt, const EntitySnapshot& snap) const {
    const auto tComp = comp_mgr.addComponent<TransformComponent>(new_entt);
    tComp->init(snap.pos_x, snap.pos_y, snap.angle);

    const auto spriteComp = comp_mgr.addComponent<BulletSpriteComponent>(new_entt);
    spriteComp->init(snap.sprite_type);
}

void EntityFactory::destroy(const Entity& entt) const { comp_mgr.removeAllComponentsOf(entt); }
