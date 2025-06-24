#include "../../../client/include/model/HUD.h"

#include <iomanip>

#include "../../../client/include/model/Graphics.h"


HUD::HUD(): numberRenderer(SpriteType::HUD_NUMBERS), symbolsRenderer(SpriteType::HUD_SYMBOLS) {
    numberRenderer.setRenderSize(SYMBOL_W, SYMBOL_H);
}

void HUD::updateFromSnapshot(const LocalPlayerInfo& player, float _timeLeft, BombState _bombState) {
    timeLeft = static_cast<int>(_timeLeft);
    health = player.health;
    money = player.money;
    ammo = player.ammo_weapon;
    bombState = _bombState;
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
    SDL_Color color;

    if (health > 60) {
        color = COLOR_GREEN;
    } else if (health > 20) {
        color = COLOR_YELLOW;
    } else {
        color = COLOR_RED;
    }
    // Dibujamos el símbolo del reloj igual que antes
    symbolsRenderer.renderSymbol(graphics, HUDSymbolType::CLOCK, MARGIN, baseY, SCALE);
    // Ahora dibujamos el texto en rojo
    numberRenderer.renderFromText(graphics, std::to_string(health), MARGIN + SYMBOL_W + TEXT_PADDING, baseY, color);
}

void HUD::renderTime(Graphics& graphics, const int screenW, const int baseY) {
    const std::string timeStr = formatTime(timeLeft);
    const Rect textBounds = numberRenderer.measureText(timeStr);
    const int totalW = SYMBOL_W + TEXT_PADDING + textBounds.GetW();
    const int centerX = (screenW - totalW) / 2;

    SDL_Color color = COLOR_WHITE;
    if (bombState == BombState::Planted) {
        color = COLOR_RED;
    } else if (bombState == BombState::Defused) {
        color = COLOR_GREEN;
    }
    // Dibujamos el símbolo del reloj igual que antes
    symbolsRenderer.renderSymbol(graphics, HUDSymbolType::CLOCK, centerX, baseY, SCALE);
    // Ahora dibujamos el texto en rojo
    numberRenderer.renderFromText(graphics, timeStr, centerX + SYMBOL_W + TEXT_PADDING, baseY, color);

}

void HUD::renderAmmo(Graphics& graphics, const int screenW, const int baseY) {
    const std::string ammoStr = formatAmmo(ammo);
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


std::string HUD::formatAmmo(const int& ammo) {
    std::ostringstream oss;
    oss << std::setfill('0') << ammo;

    return oss.str();
}
