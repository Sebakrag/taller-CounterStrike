#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <vector>

#include "client/include/model/Camera.h"
#include "client/include/model/EC/ComponentManager.h"
#include "client/include/model/Graphics.h"

class WeaponSpriteComponent;

class RenderSystem {
private:
    struct EquippedWeaponsEntry {
        Entity weaponID;
        WeaponSpriteComponent& weaponSpr;

        EquippedWeaponsEntry(const Entity id, const WeaponSpriteComponent& spr):
                weaponID(id), weaponSpr(spr) {}
    };
    std::vector<EquippedWeaponsEntry> equippedWeaponsSprite;

    void renderDroppedWeapons(Graphics& graphics, ComponentManager& comp_mgr, const Camera& camera);
    // void renderBullets(Graphics& graphics, ComponentManager& comp_mgr, const Camera& camera);
    void renderPlayers(Graphics& graphics, ComponentManager& comp_mgr, const Camera& camera);
    void renderEquippedWeapons(Graphics& graphics, ComponentManager& comp_mgr,
                               const Camera& camera);

public:
    RenderSystem() = default;

    void renderEntities(Graphics& graphics, ComponentManager& comp_mgr, const Camera& camera);
};

#endif  // RENDERSYSTEM_H
