#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>

#include "Entity.h"

// This is the equivalent to the World of the game.
class EntityManager {
private:
    Entity nextId = 1;
    std::vector<Entity> freeIds;

public:
    Entity create_entity() {
        if (!freeIds.empty()) {
            Entity e = freeIds.back();
            freeIds.pop_back();
            return e;
        }
        return nextId++;
    }

    void destroy_entity(Entity entity) {
        // Si elimino una entidad entonces deberia eliminar todos sus componentes asociados.
        // QUizas que EntityManager deberia tener una referencia a ComponentManager.
        freeIds.push_back(entity);
    }
};

#endif  // ENTITYMANAGER_H
