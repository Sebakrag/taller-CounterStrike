#include "client/include/model/EC/RenderSystem.h"

#include <cmath>

#include "client/client_constants.h"
#include "client/include/model/EC/components/BulletSpriteComponent.h"
#include "client/include/model/EC/components/EquippedWeaponComponent.h"
#include "client/include/model/EC/components/PlayerSpriteComponent.h"
#include "client/include/model/EC/components/TransformComponent.h"
#include "client/include/model/EC/components/WeaponSpriteComponent.h"
#include "common/utils/AngleUtils.h"
#include "model/EC/components/BombSpriteComponent.h"

RenderSystem::RenderSystem(const Entity localPlayer): local_player(localPlayer) {}


void RenderSystem::renderEntities(Graphics& graphics, ComponentManager& comp_mgr,
                                  const Camera& camera, const FieldOfView& player_FOV) {
    renderDroppedWeapons(graphics, comp_mgr, camera, player_FOV);
    renderBomb(graphics, comp_mgr, camera, player_FOV);
    renderBullets(graphics, comp_mgr, camera, player_FOV);
    renderPlayers(graphics, comp_mgr, camera, player_FOV);
}

void RenderSystem::renderDroppedWeapons(Graphics& graphics, ComponentManager& comp_mgr,
                                        const Camera& camera, const FieldOfView& player_FOV) {
    comp_mgr.forEach<WeaponSpriteComponent>([&](const WeaponSpriteComponent& weaponSpr,
                                                const Entity e) {
        if (weaponSpr.getState() != WeaponState::DROPPED) {
            return;
        }

        const auto transform = comp_mgr.getComponent<TransformComponent>(e);
        if (!transform)
            return;

        const Vec2D& enttMapPos = transform->getPosition();  // Posición absoluta en el mapa.

        const int width = weaponSpr.getWidth();
        const int height = weaponSpr.getHeight();

        if (!camera.isVisible(enttMapPos, width, height) || !player_FOV.isInFOV(enttMapPos))
            return;

        // Calculamos la posición de la entidad relativa a la cámara
        const Vec2D screenPos = camera.projectToScreen(enttMapPos, width, height);

        Rect destRect(static_cast<int>(screenPos.getX()), static_cast<int>(screenPos.getY()),
                      width * RENDER_SCALAR, height * RENDER_SCALAR);

        graphics.draw(*weaponSpr.getTexture(), Optional<Rect>(weaponSpr.getSpriteRect()),
                      Optional<Rect>(destRect));
    });
}


void RenderSystem::renderBomb(Graphics& graphics, ComponentManager& comp_mgr, const Camera& camera,
                              const FieldOfView& player_FOV) {
    comp_mgr.forEach<BombSpriteComponent>([&](const BombSpriteComponent& bombSpr, const Entity e) {
        const BombState state = bombSpr.getState();
        if ((state == BombState::Hidden) || (state == BombState::Equipped)) {
            return;
        }

        const auto transform = comp_mgr.getComponent<TransformComponent>(e);
        if (!transform)
            return;

        const Vec2D& enttMapPos = transform->getPosition();  // Posición absoluta en el mapa.

        const int width = bombSpr.getWidth();
        const int height = bombSpr.getHeight();

        if (!camera.isVisible(enttMapPos, width, height) || !player_FOV.isInFOV(enttMapPos))
            return;

        // Calculamos la posición de la entidad relativa a la cámara
        const Vec2D screenPos = camera.projectToScreen(enttMapPos, width, height);

        Rect destRect(static_cast<int>(screenPos.getX()), static_cast<int>(screenPos.getY()),
                      width * RENDER_SCALAR, height * RENDER_SCALAR);

        graphics.draw(*bombSpr.getTexture(), Optional<Rect>(bombSpr.getSpriteRect()),
                      Optional<Rect>(destRect));
    });
}

void RenderSystem::renderBullets(Graphics& graphics, ComponentManager& comp_mgr,
                                 const Camera& camera, const FieldOfView& player_FOV) {
    comp_mgr.forEach<BulletSpriteComponent>([&](const BulletSpriteComponent& bulletSpr,
                                                const Entity e) {
        const auto transform = comp_mgr.getComponent<TransformComponent>(e);
        if (!transform)
            return;

        const Vec2D& enttMapPos = transform->getPosition();  // Posición absoluta en el mapa.

        const int width = bulletSpr.getWidth();
        const int height = bulletSpr.getHeight();

        if (!camera.isVisible(enttMapPos, width, height) || !player_FOV.isInFOV(enttMapPos))
            return;

        // Calculamos la posición de la entidad relativa a la cámara
        const Vec2D screenPos = camera.projectToScreen(enttMapPos, width, height);

        Rect destRect(static_cast<int>(screenPos.getX()), static_cast<int>(screenPos.getY()), width,
                      height);

        const double rotAngleDeg = transform->getRotationAngleDegrees();

        graphics.draw(*bulletSpr.getTexture(), Optional<Rect>(bulletSpr.getSpriteRect()),
                      Optional<Rect>(destRect), rotAngleDeg);
    });
}

void RenderSystem::renderPlayers(Graphics& graphics, ComponentManager& comp_mgr,
                                 const Camera& camera, const FieldOfView& player_FOV) {
    comp_mgr.forEach<PlayerSpriteComponent>([&](const PlayerSpriteComponent& playerSpr,
                                                const Entity e) {
        if (playerSpr.getState() == PlayerState::Dead)
            return;

        const auto transform = comp_mgr.getComponent<TransformComponent>(e);
        if (!transform)
            return;

        const Vec2D& playerMapPos = transform->getPosition();  // Posición absoluta en el mapa.

        const int width = playerSpr.getWidth();
        const int height = playerSpr.getHeight();

        if ((e != local_player) &&
            (!camera.isVisible(playerMapPos, width, height) || !player_FOV.isInFOV(playerMapPos)))
            return;

        // Calculamos la posición de la entidad relativa a la cámara
        const Vec2D screenPos = camera.projectToScreen(playerMapPos, width, height);

        Rect playerDestRect(static_cast<int>(screenPos.getX()), static_cast<int>(screenPos.getY()),
                            width * RENDER_SCALAR, height * RENDER_SCALAR);

        const float rotAngleDeg = transform->getRotationAngleDegrees();
        // Renderizo el player.
        graphics.draw(*playerSpr.getTexture(), Optional<Rect>(playerSpr.getSpriteRect()),
                      Optional<Rect>(playerDestRect), rotAngleDeg);

        // Verificamos si tiene un arma equipada y la renderizamos junto con el jugador.
        const auto equippedWeapon = comp_mgr.getComponent<EquippedWeaponComponent>(e);
        Entity weaponID = INVALID_ENTITY;
        if (equippedWeapon && ((weaponID = equippedWeapon->getID()) != INVALID_ENTITY)) {
            renderEquippedWeapon(graphics, comp_mgr, weaponID, playerDestRect, rotAngleDeg);
        }
    });
}

void RenderSystem::renderEquippedWeapon(Graphics& graphics, ComponentManager& comp_mgr,
                                        const Entity weapon_id, const Rect& player_dest_rect,
                                        const float rotAngleDeg) {
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
        const float radians = AngleUtils::degreesToRadians(rotAngleDeg);
        const float rx = ox * std::cos(radians) - oy * std::sin(radians);
        const float ry = ox * std::sin(radians) + oy * std::cos(radians);

        // Posición final del arma
        const float weaponX = centerX + rx - (static_cast<float>(width) * RENDER_SCALAR) / 2.0f;
        const float weaponY = centerY + ry - (static_cast<float>(height) * RENDER_SCALAR) / 2.0f;

        Rect weaponDestRect(static_cast<int>(weaponX), static_cast<int>(weaponY),
                            width * RENDER_SCALAR, height * RENDER_SCALAR);

        graphics.draw(*weaponSpr->getTexture(), Optional<Rect>(weaponSpr->getSpriteRect()),
                      Optional<Rect>(weaponDestRect), rotAngleDeg);
    } else if (const auto bombSpr = comp_mgr.getComponent<BombSpriteComponent>(weapon_id)) {
        const Vec2D offset = bombSpr->getRenderOffset();
        const auto width = bombSpr->getWidth();
        const auto height = bombSpr->getHeight();

        // Centro del jugador (desde su rectángulo destino en pantalla)
        const Point playerCenter = player_dest_rect.GetCentroid();
        const auto centerX = static_cast<float>(playerCenter.GetX());
        const auto centerY = static_cast<float>(playerCenter.GetY());

        // Offset escalado
        const float ox = offset.getX() * RENDER_SCALAR;
        const float oy = -offset.getY() * RENDER_SCALAR;  // eje Y invertido en pantalla

        // Rotar offset alrededor del centro del jugador
        const float radians = AngleUtils::degreesToRadians(rotAngleDeg);
        const float rx = ox * std::cos(radians) - oy * std::sin(radians);
        const float ry = ox * std::sin(radians) + oy * std::cos(radians);

        // Posición final del arma
        const float weaponX = centerX + rx - (static_cast<float>(width) * RENDER_SCALAR) / 2.0f;
        const float weaponY = centerY + ry - (static_cast<float>(height) * RENDER_SCALAR) / 2.0f;

        Rect weaponDestRect(static_cast<int>(weaponX), static_cast<int>(weaponY),
                            width * RENDER_SCALAR, height * RENDER_SCALAR);

        graphics.draw(*bombSpr->getTexture(), Optional<Rect>(bombSpr->getSpriteRect()),
                      Optional<Rect>(weaponDestRect), rotAngleDeg);
    }
}
