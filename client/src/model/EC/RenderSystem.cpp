#include "client/include/model/EC/RenderSystem.h"

#include <cmath>

#include "client/include/model/EC/components/EquippedWeaponComponent.h"
#include "client/include/model/EC/components/PlayerSpriteComponent.h"
#include "client/include/model/EC/components/TransformComponent.h"
#include "client/include/model/EC/components/WeaponSpriteComponent.h"

#define RENDER_SCALAR 2

void RenderSystem::renderEntities(Graphics& graphics, ComponentManager& comp_mgr,
                                  const Camera& camera) {
    renderDroppedWeapons(graphics, comp_mgr, camera);
    // renderBullets(graphics, comp_mgr, camera);
    renderPlayers(graphics, comp_mgr, camera);
}

void RenderSystem::renderDroppedWeapons(Graphics& graphics, ComponentManager& comp_mgr,
                                        const Camera& camera) {
    comp_mgr.forEach<WeaponSpriteComponent>([&](WeaponSpriteComponent& weaponSpr, const Entity e) {
        if (weaponSpr.getState() != WeaponState::DROPPED) {
            return;
        }

        const auto transform = comp_mgr.getComponent<TransformComponent>(e);
        if (!transform)
            return;

        const Vec2D& enttMapPos = transform->getPosition();  // Posición absoluta en el mapa.

        const int width = weaponSpr.getWidth();
        const int height = weaponSpr.getHeight();

        if (!camera.isVisible(enttMapPos, width, height))
            return;

        // Calculamos la posición de la entidad relativa a la cámara
        const Vec2D screenPos = camera.projectToScreen(enttMapPos, width, height);

        Rect destRect(static_cast<int>(screenPos.getX()), static_cast<int>(screenPos.getY()),
                      width * RENDER_SCALAR, height * RENDER_SCALAR);

        graphics.draw(*weaponSpr.getTexture(), Optional<Rect>(weaponSpr.getSpriteRect()),
                      Optional<Rect>(destRect));
    });
}

// void RenderSystem::renderBullets(Graphics& graphics, ComponentManager& comp_mgr, const Camera&
// camera) {
//     comp_mgr.forEach<BulletSpriteComponent>([&](BulletSpriteComponent& bulletSpr, const Entity e)
//     {
//         const auto transform = comp_mgr.getComponent<TransformComponent>(e);
//         if (!transform) return;
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

        Rect playerDestRect(static_cast<int>(screenPos.getX()), static_cast<int>(screenPos.getY()),
                            width * RENDER_SCALAR, height * RENDER_SCALAR);

        const double rotAngle = transform->getRotationAngle();
        // Renderizo el player.
        graphics.draw(*playerSpr.getTexture(), Optional<Rect>(playerSpr.getSpriteRect()),
                      Optional<Rect>(playerDestRect), rotAngle);

        // Verificamos si tiene un arma equipada y la renderizamos junto con el jugador.
        const auto equippedWeapon = comp_mgr.getComponent<EquippedWeaponComponent>(e);
        Entity weaponID = INVALID_ENTITY;
        if (equippedWeapon && ((weaponID = equippedWeapon->getID()) != INVALID_ENTITY)) {
            renderEquippedWeapon(graphics, comp_mgr, weaponID, playerDestRect, rotAngle);
        }
    });
}

void RenderSystem::renderEquippedWeapon(Graphics& graphics, ComponentManager& comp_mgr,
                                        const Entity weapon_id, const Rect& player_dest_rect,
                                        const double rotAngle) {
    if (const auto weaponSpr = comp_mgr.getComponent<WeaponSpriteComponent>(weapon_id)) {
        const Vec2D offset = weaponSpr->getRenderOffset();
        const auto width = weaponSpr->getWidth();
        const auto height = weaponSpr->getHeight();

        // Centro del jugador (desde su rectángulo destino en pantalla)
        const Point playerCenter = player_dest_rect.GetCentroid();
        const auto centerX = static_cast<float>(playerCenter.GetX());
        const auto centerY = static_cast<float>(playerCenter.GetY());

        // Offset escalado
        const float ox = offset.getX() * RENDER_SCALAR;
        const float oy = -offset.getY() * RENDER_SCALAR;  // eje Y invertido en pantalla

        // Rotar offset alrededor del centro del jugador
        const float radians = static_cast<float>(rotAngle) * static_cast<float>(M_PI) / 180.0f;
        const float rx = ox * std::cos(radians) - oy * std::sin(radians);
        const float ry = ox * std::sin(radians) + oy * std::cos(radians);

        // Posición final del arma
        const float weaponX = centerX + rx - (static_cast<float>(width) * RENDER_SCALAR) / 2.0f;
        const float weaponY = centerY + ry - (static_cast<float>(height) * RENDER_SCALAR) / 2.0f;

        Rect weaponDestRect(static_cast<int>(weaponX), static_cast<int>(weaponY),
                            height * RENDER_SCALAR, height * RENDER_SCALAR);

        graphics.draw(*weaponSpr->getTexture(), Optional<Rect>(weaponSpr->getSpriteRect()),
                      Optional<Rect>(weaponDestRect), rotAngle);
    }
}
