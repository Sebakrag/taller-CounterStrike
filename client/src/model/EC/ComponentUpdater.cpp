#include "client/include/model/EC/ComponentUpdater.h"

#include "client/include/model/EC/components/EquippedWeaponComponent.h"
#include "client/include/model/EC/components/PlayerSpriteComponent.h"
#include "client/include/model/EC/components/SoundComponent.h"
#include "client/include/model/EC/components/SpriteComponent.h"
#include "client/include/model/EC/components/TransformComponent.h"
#include "client/include/model/EC/components/WeaponSpriteComponent.h"
#include "client/include/model/utils/SoundEvent.h"

ComponentUpdater::ComponentUpdater(EntityManager& em, ComponentManager& cm):
        entt_mgr(em), comp_mgr(cm) {
    old_entities.reserve(INITIAL_OLD_ENTITIES_SIZE);
}

void ComponentUpdater::update(const std::vector<EntitySnapshot>& snapshots) {
    syncEntities(snapshots);
    updateComponents();
}

void ComponentUpdater::syncEntities(const std::vector<EntitySnapshot>& snapshots) {
    if (old_entities.capacity() < snapshots.size()) {
        old_entities.reserve(snapshots.size());
    }
    old_entities
            .clear();  // OJO: Si agrego un campo de tipo std::string en EntitySnapshot, entonces el
    // struct OldEntityEntry puede dejar de ser trivially_destructible y la
    // operacion old_entities.clear() puede pasar de ser O(1) a O(n).

    for (const auto& snap: snapshots) {
        if (!snap.is_alive) {  // I have to destroy it. Later we could have an enum that indicates
                               // if have to delete it immediately or not.
            entt_mgr.destroy_entity_immediately(snap.server_entt_id);
            continue;
        }

        Entity e = entt_mgr.get(snap.server_entt_id);
        if (e == INVALID_ENTITY) {
            entt_mgr.create_entity(
                    snap);  // TODO: This could throw an error (where we should handle it?).
        } else {
            old_entities.emplace_back(e, snap);
        }
    }
}

void ComponentUpdater::updateComponents() {
    // Here I can just use the old_entities vector (because all the new entities are already
    // updated, since they were just created).
    for (const auto& [e, snap]: old_entities) {
        if (const auto transform = comp_mgr.getComponent<TransformComponent>(e)) {
            transform->update(snap.pos_x, snap.pos_y, snap.angle);
        }
        if (const auto equippedWeapon = comp_mgr.getComponent<EquippedWeaponComponent>(e)) {
            if (const auto player = std::get_if<PlayerSnapshot>(&snap.data)) {
                equippedWeapon->setID(entt_mgr.get(player->equipped_weapon_id));
            }
        }
        if (const auto weaponSpr = comp_mgr.getComponent<WeaponSpriteComponent>(e)) {
            if (const auto weapon = std::get_if<WeaponSnapshot>(&snap.data)) {
                weaponSpr->setState(weapon->state);
            }
        }
        if (const auto playerSpr = comp_mgr.getComponent<PlayerSpriteComponent>(e)) {
            if (const auto player = std::get_if<PlayerSnapshot>(&snap.data)) {
                playerSpr->setTypeWeaponEquipped(player->weapon_type);
            }
        }
        if (const auto soundComp = comp_mgr.getComponent<SoundComponent>(e)) {
            if (const auto player = std::get_if<PlayerSnapshot>(&snap.data)) {
                updatePlayerSoundComponent(e, *soundComp, *player, {snap.pos_x, snap.pos_y});
            }
        }

        // Deberia crear un AnimationComponent para actualizar el sprite de forma correcta.
        // if (const auto spr = comp_mgr.getComponent<SpriteComponent>(e)) {
        //     spr->update(snap.pos_x, snap.pos_y);
        // }
    }
}

void ComponentUpdater::updatePlayerSoundComponent(const Entity e, SoundComponent& soundComp,
                                                  const PlayerSnapshot& playerSnap,
                                                  const Vec2D& curr_pos) {
    static bool already_dead = false;
    const Entity curr_weapon = entt_mgr.get(playerSnap.equipped_weapon_id);
    const PlayerState curr_state = playerSnap.state;
    const int curr_health = playerSnap.hp;

    if (const auto it = previous_player_info.find(e); it != previous_player_info.end()) {
        const auto& prev_info = it->second;

        Vec2D dpos(curr_pos - prev_info.position);
        if (dpos.calculateNormSquared() > MIN_MOVEMENT_EPSILON * MIN_MOVEMENT_EPSILON)
            soundComp.addEvent(SoundEvent::Walk);

        if (prev_info.weapon_id != curr_weapon)
            soundComp.addEvent(SoundEvent::ChangeWeapon);

        std::cout << "[Previous HP]: " << prev_info.health << "[Current HP]: " << curr_health
                  << std::endl;
        // if (curr_health <= 0) {
        //     soundComp.addEvent(SoundEvent::Die);
        // }
        if (curr_health < prev_info.health) {
            soundComp.addEvent(SoundEvent::TakeDamage);
        }

        std::cout << "[Previous State]: " << static_cast<int>(prev_info.state)
                  << "[Current State]: " << static_cast<int>(curr_state) << std::endl;

        if (curr_state == PlayerState::Attacking) {
            // Quizas que deberia enviar un evento de sonido al componente de su arma.
            // (en el caso de que querramos distinguir sonidos de arma.)
            soundComp.addEvent(SoundEvent::Shoot);
        } else if (curr_state == PlayerState::PickingUp) {
            soundComp.addEvent(SoundEvent::DropWeapon);
            soundComp.addEvent(SoundEvent::PickUpWeapon);  // TODO: Este quizas lo podemos sacar.
        } else if (!already_dead && (curr_state == PlayerState::Dead)) {
            soundComp.addEvent(SoundEvent::Die);
            already_dead = true;
        }
    }

    // Guardar snapshot actual para próxima comparación
    // previous_player_info[e] = {curr_state, curr_weapon, curr_pos};
    previous_player_info.insert_or_assign(
            e, PreviousPlayerInfo{curr_state, curr_weapon, curr_pos, curr_health});
}
