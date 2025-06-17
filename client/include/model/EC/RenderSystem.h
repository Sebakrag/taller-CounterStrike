#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "client/include/model/Camera.h"
#include "client/include/model/EC/ComponentManager.h"
#include "client/include/model/FieldOfView.h"
#include "client/include/model/Graphics.h"

class RenderSystem {
private:
    Entity local_player;
    void renderDroppedWeapons(Graphics& graphics, ComponentManager& comp_mgr, const Camera& camera,
                              const FieldOfView& player_FOV);
    // void renderBullets(Graphics& graphics, ComponentManager& comp_mgr, const Camera& camera,
    // const FieldOfView& player_FOV);
    void renderPlayers(Graphics& graphics, ComponentManager& comp_mgr, const Camera& camera,
                       const FieldOfView& player_FOV);
    void renderEquippedWeapon(Graphics& graphics, ComponentManager& comp_mgr, Entity weapon_id,
                              const Rect& player_dest_rect, float rotAngleDeg);

public:
    explicit RenderSystem(Entity localPlayer);

    void renderEntities(Graphics& graphics, ComponentManager& comp_mgr, const Camera& camera,
                        const FieldOfView& player_FOV);
};

#endif  // RENDERSYSTEM_H
