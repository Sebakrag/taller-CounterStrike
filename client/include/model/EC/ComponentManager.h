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

    template <typename Component>
    ComponentPool<Component>* getPool() {
        std::type_index index(typeid(Component));
        if (!pools_map.contains(index)) {
            pools_map[index] = std::make_unique<ComponentPool<Component>>();
        }
        return static_cast<ComponentPool<Component>*>(pools_map[index].get());
    }

public:
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
};

#endif  // COMPONENTMANAGER_H
