#include "client/include/model/EC/ComponentUpdater.h"

#include "client/include/model/EC/components/PositionComponent.h"

ComponentUpdater::ComponentUpdater(EntityManager& em, ComponentManager& cm):
        entt_mgr(em), comp_mgr(cm), old_entities(INITIAL_OLD_ENTITIES_SIZE) {}

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

// This method should be private. It doesn't make sense that we can call this without having
// the old_entities container updated (we must call syncEntity before calling applySnapshotData).
// Another option is to document this clearly and explain when should be used it.
void ComponentUpdater::applySnapshotData() {
    // here I must use the old_entities container.
    for (const auto& [e, snap]: old_entities) {
        if (const auto pos = comp_mgr.getComponent<PositionComponent>(e)) {
            pos->init(snap.pos_x, snap.pos_y);
        }
        // if (const auto spr = comp_mgr.getComponent<SpriteComponent>(e)) {
        //     spr->setTexture(snap.sprite_type);
        // }
    }
}

void ComponentUpdater::updateComponents() {
    // comp_mgr.update<SpriteComponent>();
    // updateSpriteComponents();
}

// void ComponentUpdater::updateSpriteComponents() {
//     // comp_mgr.forEach<SpriteComponent>([](SpriteComponent& comp) {
//     //     comp->update();
//     // });
// }
