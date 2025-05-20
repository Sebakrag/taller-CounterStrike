#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "ComponentStorage.h"
#include "Entity.h"

class ComponentManager {
private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> componentArrays;

    template <typename T>
    std::shared_ptr<ComponentArray<T>> getArray() {
        const auto type = std::type_index(typeid(T));
        if (!componentArrays.contains(type)) {
            componentArrays[type] = std::make_shared<ComponentArray<T>>();
        }
        return std::static_pointer_cast<ComponentArray<T>>(componentArrays[type]);
    }

public:
    template <typename T>
    void addComponent(const Entity e, const T& component) {
        getArray<T>()->insert(e, component);
    }

    template <typename T>
    void removeComponent(const Entity e) {
        getArray<T>()->remove(e);
    }

    template <typename T>
    T* getComponent(const Entity e) {
        return getArray<T>()->get(e);
    }

    template <typename T>
    ComponentArray<T>& getComponentArray() {
        return *getArray<T>();
    }
};

#endif  // COMPONENTMANAGER_H
