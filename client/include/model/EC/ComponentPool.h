#ifndef COMPONENTPOOL_H
#define COMPONENTPOOL_H

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "Entity.h"


class BasePool {
public:
    virtual ~BasePool() = default;
    virtual void remove(Entity entity) = 0;
};

template <typename T>
class ComponentPool: public BasePool {
public:
    explicit ComponentPool(size_t capacity = MAX_ENTITIES) {
        pool.reserve(capacity);
        entities.reserve(capacity);
    }

    T* create(const Entity e) {
        if (has(e)) {
            return get(e);
        }

        size_t index;
        if (!free_indices.empty()) {
            index = free_indices.back();
            free_indices.pop_back();

            pool[index] = T();
            entities[index] = e;
        } else {
            index = pool.size();
            pool.emplace_back();  // Default constructor
            entities.emplace_back(e);
        }

        entity_to_index[e] = index;
        return &pool[index];
    }

    void remove(const Entity e) override {
        const auto it = entity_to_index.find(e);
        if (it == entity_to_index.end())
            return;

        size_t index = it->second;
        entity_to_index.erase(it);

        // Optional: reset values to default (clean memory)
        pool[index] = T();
        entities[index] = INVALID_ENTITY;

        free_indices.push_back(index);
    }

    T* get(const Entity e) {
        const auto it = entity_to_index.find(e);
        if (it == entity_to_index.end())
            return nullptr;
        return &pool[it->second];
    }

    bool has(const Entity e) const { return entity_to_index.contains(e); }

    void clear() {
        pool.clear();
        entities.clear();
        entity_to_index.clear();
        free_indices.clear();
    }

    template <typename Func>
    void forEach(Func func) {
        for (size_t i = 0; i < pool.size(); ++i) {
            if (entities[i] != INVALID_ENTITY) {
                func(pool[i], entities[i]);
            }
        }
    }

private:
    std::vector<T> pool;
    std::vector<Entity> entities;                        // index -> Entity (sirve solo para iterar)
    std::unordered_map<Entity, size_t> entity_to_index;  // Entity -> pool index
    std::vector<size_t> free_indices;

    const uint32_t INVALID_INDEX = INVALID_ENTITY;
};

#endif  // COMPONENTPOOL_H
