#ifndef COMPONENTSTORAGE_H
#define COMPONENTSTORAGE_H

#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "Entity.h"

template <typename T>
class ComponentArray {
private:
    std::vector<T> data;  // Componentes contiguos. Quizas deberia almacenar un puntero inteligente
    // std::vector<std::unique_ptr<T>> data;                      // Componentes contiguos. Quizas
    // deberia almacenar un puntero inteligente
    std::vector<Entity> entities;            // Índice → Entity
    std::unordered_map<Entity, size_t> map;  // Entity → Índice

public:
    void insert(const Entity e, const T& component) {
        if (map.contains(e))
            throw std::runtime_error("Entity already has a component");
        map[e] = data.size();
        data.push_back(component);
        entities.push_back(e);
    }

    void remove(const Entity e) {
        const auto it = map.find(e);
        if (it == map.end())
            throw std::runtime_error("Entity doesn't have a component");

        size_t idx = it->second;
        size_t lastIdx = data.size() - 1;

        // Swap con el último para mantener el vector compacto
        data[idx] = data[lastIdx];
        entities[idx] = entities[lastIdx];

        map[entities[idx]] = idx;  // Actualizar map de la entidad que quedó en idx
        data.pop_back();
        entities.pop_back();
        map.erase(e);
    }

    T* get(const Entity e) {
        const auto it = map.find(e);
        return (it != map.end()) ? &data[it->second] : nullptr;
    }

    std::vector<T>& getData() { return data; }
    std::vector<Entity>& getEntities() { return entities; }
};

#endif  // COMPONENTSTORAGE_H
