#include "client/include/model/HUD.h"

#include <iomanip>

#include "client/include/model/Graphics.h"


HUD::HUD(): numberRenderer(SpriteType::HUD_NUMBERS), symbolsRenderer(SpriteType::HUD_SYMBOLS) {
    numberRenderer.setRenderSize(SYMBOL_W, SYMBOL_H);
}

void HUD::updateFromSnapshot(const EntitySnapshot& snap) {
    // TODO: Recibir el timeLeft en segundos.
    timeLeft++;
    if (const auto player = std::get_if<PlayerSnapshot>(&snap.data)) {
        health = player->hp;
        money = player->money;
        ammoInfo.totalAmmo =
                player->ammo;  // TODO: eliminar ammo de player. AHora hay que obtenerlo del arma.
        ammoInfo.ammoLoaded = 5;
    }
}

void HUD::render(Graphics& graphics) {
    const Vec2D winDimension = graphics.getDrawableWindowDimension();
    const int screenW = static_cast<int>(winDimension.getX());
    const int screenH = static_cast<int>(winDimension.getY());

    const int baseY = screenH - SYMBOL_H - MARGIN;

    renderHealth(graphics, baseY);
    renderTime(graphics, screenW, baseY);
    renderAmmo(graphics, screenW, baseY);
    renderMoney(graphics, screenW, baseY);
}

void HUD::renderSymbolAndText(Graphics& graphics, const HUDSymbolType symbol,
                              const std::string& text, const int x, const int y) {
    symbolsRenderer.renderSymbol(graphics, symbol, x, y, SCALE);
    numberRenderer.renderFromText(graphics, text, x + SYMBOL_W + TEXT_PADDING, y);
}

void HUD::renderHealth(Graphics& graphics, const int baseY) {
    renderSymbolAndText(graphics, HUDSymbolType::HEALTH, std::to_string(health), MARGIN, baseY);
}

void HUD::renderTime(Graphics& graphics, const int screenW, const int baseY) {
    const std::string timeStr = formatTime(timeLeft);
    const Rect textBounds = numberRenderer.measureText(timeStr);
    const int totalW = SYMBOL_W + TEXT_PADDING + textBounds.GetW();
    const int centerX = (screenW - totalW) / 2;

    renderSymbolAndText(graphics, HUDSymbolType::CLOCK, timeStr, centerX, baseY);
}

void HUD::renderAmmo(Graphics& graphics, const int screenW, const int baseY) {
    const std::string ammoStr = formatAmmo(ammoInfo);
    const Rect textBounds = numberRenderer.measureText(ammoStr);
    const int totalW = SYMBOL_W + TEXT_PADDING + textBounds.GetW();
    const int rightX = screenW - totalW - MARGIN;

    renderSymbolAndText(graphics, HUDSymbolType::NONE, ammoStr, rightX, baseY);
}

void HUD::renderMoney(Graphics& graphics, const int screenW, const int baseY) {
    const std::string moneyStr = std::to_string(money);
    const Rect textBounds = numberRenderer.measureText(moneyStr);
    const int totalW = SYMBOL_W + TEXT_PADDING + textBounds.GetW();
    const int rightX = screenW - totalW - MARGIN;
    const int moneyY = baseY - SYMBOL_H - MARGIN;

    renderSymbolAndText(graphics, HUDSymbolType::MONEY, moneyStr, rightX, moneyY);
}

std::string HUD::formatTime(const int seconds) {
    const int minutes = seconds / 60;
    const int secs = seconds % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0')
        << secs;

    return oss.str();
}


std::string HUD::formatAmmo(const AmmoInfo& ammoInfo) {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << ammoInfo.ammoLoaded << "|" << std::setw(3)
        << std::setfill('0') << ammoInfo.totalAmmo;

    return oss.str();
}


/// TODO: Refactorizar utilizando componentes.
/// Version de update utilizando ComponentManager:
/// void HUD::update(ComponentManager& comp_mgr, Entity player_id) {
///     if (auto healthComp = comp_mgr.getComponent<HealthComponent>(player_id)) {
///         health = healthComp->getCurrent();
///     }
///
///     if (auto weapon = comp_mgr.getComponent<EquippedWeaponComponent>(player_id)) {
///         Entity weapon_id = weapon->getID();
///         if (auto ammoComp = comp_mgr.getComponent<AmmoComponent>(weapon_id)) {
///             ammoInfo = ammoComp->getAmmoInfo();
///         }
///     }
///
///     if (auto moneyComp = comp_mgr.getComponent<MoneyComponent>(player_id)) {
///         money = moneyComp->getAmount();
///     }
/// }
