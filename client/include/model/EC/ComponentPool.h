#ifndef COMPONENTPOOL_H
#define COMPONENTPOOL_H

#include <array>
#include <bitset>

#include "Entity.h"

class BasePool {
public:
    virtual ~BasePool() = default;
};

template <typename T>
class ComponentPool: public BasePool {
public:
    ComponentPool() {
        entity_to_pool_index.fill(INVALID_ENTITY);
        entities.fill(INVALID_ENTITY);
    }

    T* create(const Entity entity) {
        if (entity_to_pool_index[entity] != INVALID_ENTITY) {
            return &pool[entity_to_pool_index[entity]];  // The entity already has the component
        }

        for (size_t i = 0; i < pool.size(); ++i) {
            if (!in_use[i]) {
                in_use[i] = true;
                entities[i] = entity;
                entity_to_pool_index[entity] = i;
                return &pool[i];
            }
        }
        return nullptr;  // There is no memory available for a new component
    }

    void remove(const Entity entity) {
        Entity index = entity_to_pool_index[entity];
        if (index == INVALID_ENTITY)
            return;  // The entity doesn't have this kind of component.

        in_use[index] = false;
        entity_to_pool_index[entity] = INVALID_ENTITY;
        entities[index] = INVALID_ENTITY;

        if constexpr (requires(T t) { t.init(); }) {
            pool[index].init();  // Reset to default values (optional)
        }
    }

    T* get(const Entity entity) {
        Entity index = entity_to_pool_index[entity];
        if (index == INVALID_ENTITY)
            return nullptr;
        return &pool[index];
    }

    // Iteration for a cache-friendly operation (like render, for instance).
    template <typename Func>
    void forEach(Func func) {
        for (size_t i = 0; i < MAX_ENTITIES; ++i) {
            if (in_use[i]) {
                func(pool[i], entities[i]);
            }
        }
    }

private:
    std::array<T, MAX_ENTITIES> pool;
    std::bitset<MAX_ENTITIES>
            in_use;  // each position it's a flag indicating if the component is available.
    std::array<Entity, MAX_ENTITIES> entity_to_pool_index;  // Entity -> pool index
    std::array<Entity, MAX_ENTITIES> entities;  // index -> Entity (sirve solo para iterar)
};

#endif  // COMPONENTPOOL_H
