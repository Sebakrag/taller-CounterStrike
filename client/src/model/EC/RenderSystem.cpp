#include "client/include/model/EC/RenderSystem.h"

#include "client/include/model/EC/components/TransformComponent.h"
#include "client/include/model/EC/components/WeaponSpriteComponent.h"

void RenderSystem::renderEntities(Graphics& graphics, ComponentManager& comp_mgr,
                                  const Camera& camera) {
    renderDroppedWeapons(graphics, comp_mgr, camera);
    // renderBullets(graphics, comp_mgr, camera);
    renderPlayers(graphics, comp_mgr, camera);
    renderEquippedWeapons(graphics, comp_mgr, camera);
}

void RenderSystem::renderDroppedWeapons(Graphics& graphics, ComponentManager& comp_mgr,
                                        const Camera& camera) {
    comp_mgr.forEach<WeaponSpriteComponent>([&](WeaponSpriteComponent& weaponSpr, const Entity e) {
        switch (weaponSpr.getStatus()) {
            case WeaponStatus::EQUIPPED:
                equippedWeaponsSprite.emplace_back(e, weaponSpr);
                return;
            case WeaponStatus::DROPPED:
                break;
            case WeaponStatus::HIDDEN:
            default:
                return;
        }
        // Si sale del switch significa que el estado del weapon es DROPPED.
        const auto transform = comp_mgr.getComponent<TransformComponent>(e);

        const Vec2D& enttMapPos = transform->getPosition();  // Posición absoluta en el mapa.

        const int width = weaponSpr.getWidth();
        const int height = weaponSpr.getHeight();

        if (!camera.isVisible(enttMapPos, width, height))
            return;

        // Calculamos la posición de la entidad relativa a la cámara
        const Vec2D screenPos = camera.projectToScreen(enttMapPos, width, height);

        Rect destRect(static_cast<int>(screenPos.getX()), static_cast<int>(screenPos.getY()), width,
                      height);

        graphics.draw(*weaponSpr.getTexture(), Optional<Rect>(weaponSpr.getSpriteRect()),
                      Optional<Rect>(destRect));
    });
}

// void RenderSystem::renderBullets(Graphics& graphics, ComponentManager& comp_mgr, const Camera&
// camera) {
//     comp_mgr.forEach<BulletSpriteComponent>([&](BulletSpriteComponent& bulletSpr, const Entity e)
//     {
//         const auto transform = comp_mgr.getComponent<TransformComponent>(e);
//
//         const Vec2D& enttMapPos = transform->getPosition();  // Posición absoluta en el mapa.
//
//         const int width = bulletSpr.getWidth();
//         const int height = bulletSpr.getHeight();
//
//         if (!camera.isVisible(enttMapPos, width, height))
//             return;
//
//         // Calculamos la posición de la entidad relativa a la cámara
//         const Vec2D screenPos = camera.projectToScreen(enttMapPos, width, height);
//
//         Rect destRect(static_cast<int>(screenPos.getX()), static_cast<int>(screenPos.getY()),
//         width,
//                       height);
//
//         const double rotAngle = transform->getRotationAngle();
//
//         graphics.draw(*bulletSpr.getTexture(), Optional<Rect>(bulletSpr.getSpriteRect()),
//                       Optional<Rect>(destRect), rotAngle);
//     });
// }

void RenderSystem::renderPlayers(Graphics& graphics, ComponentManager& comp_mgr,
                                 const Camera& camera) {
    comp_mgr.forEach<PlayerSpriteComponent>([&](PlayerSpriteComponent& playerSpr, const Entity e) {
        const auto transform = comp_mgr.getComponent<TransformComponent>(e);
        if (!transform)
            return;

        const Vec2D& playerMapPos = transform->getPosition();  // Posición absoluta en el mapa.

        const int width = playerSpr.getWidth();
        const int height = playerSpr.getHeight();

        if (!camera.isVisible(playerMapPos, width, height))
            return;

        // Calculamos la posición de la entidad relativa a la cámara
        const Vec2D screenPos = camera.projectToScreen(playerMapPos, width, height);

        Rect destRect(static_cast<int>(screenPos.getX()), static_cast<int>(screenPos.getY()), width,
                      height);

        const double rotAngle = transform->getRotationAngle();
        // Renderizo el player.
        graphics.draw(*playerSpr.getTexture(), Optional<Rect>(playerSpr.getSpriteRect()),
                      Optional<Rect>(destRect), rotAngle);

        // Verificamos si tiene un arma equipada y la renderizamos junto con el jugador.
        const auto equippedWeapon = comp_mgr.getComponent<EquippedWeaponComponent>(e);
        if (equippedWeapon) {
            const Entity weaponID = equippedWeapon->getID();
            const auto weaponSpr = comp_mgr.getComponent<WeaponSpriteComponent>(weaponID);
            graphics.draw(*weaponSpr.getTexture(), Optional<Rect>(weaponSpr.getSpriteRect()),
                          Optional<Rect>(destRect), rotAngle);
        }
    });
}

void RenderSystem::renderEquippedWeapons(Graphics& graphics, ComponentManager& comp_mgr,
                                         const Camera& camera) {
    for (const auto [weaponID, weaponSpr]: equippedWeaponsSprite) {
        auto const transform = comp_mgr.getComponent<TransformComponent>(weaponID);

        const Vec2D& enttMapPos = transform->getPosition();  // Posición absoluta en el mapa.

        const int width = weaponSpr.getWidth();
        const int height = weaponSpr.getHeight();

        if (!camera.isVisible(enttMapPos, width, height))
            return;

        // Calculamos la posición de la entidad relativa a la cámara
        const Vec2D screenPos = camera.projectToScreen(enttMapPos, width, height);

        Rect destRect(static_cast<int>(screenPos.getX()), static_cast<int>(screenPos.getY()), width,
                      height);

        const double rotAngle = transform->getRotationAngle();

        graphics.draw(*weaponSpr.getTexture(), Optional<Rect>(weaponSpr.getSpriteRect()),
                      Optional<Rect>(destRect), rotAngle);
    }
}
