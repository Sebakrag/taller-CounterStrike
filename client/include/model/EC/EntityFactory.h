#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "common/dtos/EntitySnapshot.h"

#include "ComponentManager.h"

///
/// @brief Attach the corresponding components to the entity type that we need to create.
/// Furthermore, it initializes the components to the right values.
///
class EntityFactory {
private:
    ComponentManager& comp_mgr;

    void create_anti_terrorist_entt(const Entity& new_entt, const EntitySnapshot& snap) const;

public:
    explicit EntityFactory(ComponentManager& cm);

    void create_specific_entity(const Entity& new_entt, const EntitySnapshot& snap) const;
};

#endif  // ENTITYFACTORY_H
