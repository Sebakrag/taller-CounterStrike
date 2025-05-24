#include "client/include/model/EC/ComponentUpdater.h"

#include "client/include/model/EC/components/PositionComponent.h"


void ComponentUpdater::syncEntities(const std::vector<EntitySnapshot>& snapshots) {
    old_entities
            .clear();  // OJO: Si agrego un campo de tipo std::string en EntitySnapshot, entonces el
                       // struct OldEntityEntry puede dejar de ser trivially_destructible y la
                       // operacion old_entities.clear() puede pasar de ser O(1) a O(n).
    old_entities.reserve(snapshots.size());

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

// This method should be private. It doesn't make sense that we can call this without having
// the old_entities container updated (we must call syncEntity before calling applySnapshotData).
// Another option is to document this clearly and explain when should be used it.
void ComponentUpdater::applySnapshotData(const std::vector<EntitySnapshot>& snapshots) {
    // here I must use the old_entities container.
    // for (const auto& [e, snap] : old_entities) {
    //     if (auto pos = cm.getComponent<PositionComponent>(e)) {
    //         pos->update(snap.pos_x, snap.pos_y);
    //     }
    //     if (auto spr = cm.getComponent<SpriteComponent>(e)) {
    //         spr->setTexture(snap.sprite_type);
    //     }
    //     // etc.
    // }
}


// Option 1:
void ComponentUpdater::updateComponents(const std::vector<EntitySnapshot>& snapshots,
                                        EntityManager& em, ComponentManager& cm) {
    Entity e;
    ServerEntityID id_from_serv;
    for (const auto snap: snapshots) {
        if ((e = em.get(snap.server_entt_id)) != INVALID_ENTITY) {
            updatePositionComponent(snap, e, cm);
            updateSpriteComponent(snap, e, cm);
        } else {  // if (e == INVALID_ENTITY) --> we need to create the entity.
            createEntity(snap, em);
        }
    }
}

void ComponentUpdater::updatePositionComponents(const EntitySnapshot& snap, const Entity& e,
                                                ComponentManager& cm) {
    auto comp = cm.getComponent<PositionComponent>(e);
    comp->update(snap.pos_x, snap.pos_y);
}

void ComponentUpdater::updateSpriteComponents(const EntitySnapshot& snap, const Entity& e,
                                              ComponentManager& cm) {
    auto comp = cm.getComponent<SpriteComponent>(e);
    comp->setTexture(snap.sprite_type);
}

void ComponentUpdater::createEntity(const EntitySnapshot& snap, EntityManager& em) {
    switch (snap.entt_type) {
        case PLAYER: {
            em.createPlayer(snap);
            break;
        }
        case BULLET: {
            em.createBullet(snap);
            break;
        }
    }
}

// Option 2:
void ComponentUpdater::updateComponents(const std::vector<EntitySnapshot>& snapshots,
                                        EntityManager& em, ComponentManager& cm) {
    syncEntities();
    updatePositionComponents(snapshots, em, cm);
    updateSpriteComponents(snapshots, em, cm);
}


void ComponentUpdater::updatePositionComponents(const std::vector<EntitySnapshot>& snapshots,
                                                EntityManager& em, ComponentManager& cm) {
    Entity e;
    auto pool = em.getPool<PositionComponent>();
    for (const auto snap: snapshots) {
        if ((e = em.get(snap.server_entt_id)) != INVALID_ENTITY) {
            auto comp = pool.get(e);
            comp->update(snap.pos_x, snap.pos_y);
        }
    }
}

void ComponentUpdater::updateSpriteComponents(const std::vector<EntitySnapshot>& snapshots,
                                              EntityManager& em, ComponentManager& cm) {
    Entity e;
    auto pos_pool = em.getPool<PositionComponent>();
    auto pool = em.getPool<SpriteComponent>();
    for (const auto snap: snapshots) {
        if ((e = em.get(snap.server_entt_id)) != INVALID_ENTITY) {
            auto pos_comp = pos_pool.get(e);
            auto comp = pool.get(e);
            comp->update(pos_comp);
        }
    }
}
