#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "ComponentPool.h"
#include "Entity.h"

class ComponentManager {
private:
    std::unordered_map<std::type_index, std::unique_ptr<BasePool>> pools_map;
    std::unordered_map<std::type_index, size_t> custom_capacities;

    template <typename Component>
    ComponentPool<Component>* getPool() {
        const std::type_index index(typeid(Component));
        if (!pools_map.contains(index)) {
            const size_t capacity =
                    custom_capacities.contains(index) ? custom_capacities[index] : MAX_ENTITIES;
            pools_map[index] = std::make_unique<ComponentPool<Component>>(capacity);
        }
        return static_cast<ComponentPool<Component>*>(pools_map[index].get());
    }

public:
    template <typename Component>
    void setCapacity(const size_t capacity) {
        custom_capacities[typeid(Component)] = capacity;
    }

    template <typename Component>
    Component* addComponent(const Entity e) {
        return getPool<Component>()->create(e);
    }

    template <typename Component>
    void removeComponent(const Entity e) {
        getPool<Component>()->remove(e);
    }

    template <typename Component>
    Component* getComponent(const Entity e) {
        return getPool<Component>()->get(e);
    }

    template <typename Component, typename Func>
    void forEach(Func func) {
        getPool<Component>()->forEach(func);
    }

    void removeAllComponentsOf(const Entity entity) {
        for (auto& [_, pool]: pools_map) {
            pool->remove(entity);
        }
    }
};

#endif  // COMPONENTMANAGER_H
