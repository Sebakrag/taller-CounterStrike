#include "client/include/model/EC/ComponentUpdater.h"

#include "client/include/model/EC/components/PositionComponent.h"
#include "client/include/model/EC/components/SpriteComponent.h"

ComponentUpdater::ComponentUpdater(EntityManager& em, ComponentManager& cm):
        entt_mgr(em), comp_mgr(cm) {
    old_entities.reserve(INITIAL_OLD_ENTITIES_SIZE);
}

void ComponentUpdater::update(const std::vector<EntitySnapshot>& snapshots) {
    syncEntities(snapshots);
    // applySnapshotData();
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
            // TODO: destroy all the components related to the entity I've just destroyed.
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

void ComponentUpdater::applySnapshotData() {
    // here I must use the old_entities container.
    for (const auto& [e, snap]: old_entities) {
        if (const auto pos = comp_mgr.getComponent<PositionComponent>(e)) {
            pos->init(snap.pos_x, snap.pos_y);
        }
        // Quizas que esta actualizacion del spritesheet esta demas. Si tengo que cambiar el
        // spritesheet deberia hacerlo cuando se que la entidad muere, o cuando sucede algo
        // extraordinario. No deberia recibirlo en todos los frames, pq en la mayoria de los casos
        // parece ser algo estatico. if (const auto spr = comp_mgr.getComponent<SpriteComponent>(e))
        // {
        //     spr->setTexture(snap.sprite_type);
        // }
    }
}

void ComponentUpdater::updateComponents() {
    // Here I can just use the old_entities vector (because all the new entities are already
    // updated, since they were just created).
    for (const auto& [e, snap]: old_entities) {
        if (const auto pos = comp_mgr.getComponent<PositionComponent>(e)) {
            pos->update(snap.pos_x, snap.pos_y);
        }

        if (const auto spr = comp_mgr.getComponent<SpriteComponent>(e)) {
            spr->update(snap.pos_x, snap.pos_y);
        }
    }
}
