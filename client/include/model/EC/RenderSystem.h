#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "client/include/model/Camera.h"
#include "client/include/model/EC/ComponentManager.h"
#include "client/include/model/Graphics.h"

class RenderSystem {
private:
    void renderDroppedWeapons(Graphics& graphics, ComponentManager& comp_mgr, const Camera& camera);
    // void renderBullets(Graphics& graphics, ComponentManager& comp_mgr, const Camera& camera);
    void renderPlayers(Graphics& graphics, ComponentManager& comp_mgr, const Camera& camera);

public:
    RenderSystem() = default;

    void renderEntities(Graphics& graphics, ComponentManager& comp_mgr, const Camera& camera);
};

#endif  // RENDERSYSTEM_H
