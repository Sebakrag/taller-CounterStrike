#ifndef HUD_H
#define HUD_H

#include <string>

#include "common/dtos/AmmoInfo.h"
#include "common/dtos/EntitySnapshot.h"
#include "utils/NumberRenderer.h"
#include "utils/SymbolsRenderer.h"


class Graphics;

/////
/// @brief Head-Up Display for the local player.
/////
class HUD {
private:
    NumberRenderer numberRenderer;
    SymbolsRenderer symbolsRenderer;
    int health = 0;
    int money = 0;
    AmmoInfo ammoInfo;
    int timeLeft = 125;  // in seconds

    static constexpr float SCALE = 0.5f;
    static constexpr int MARGIN = 10;
    static constexpr int TEXT_PADDING = 10;

    const int SYMBOL_W = static_cast<int>(static_cast<float>(symbolsRenderer.getSymbolW()) * SCALE);
    const int SYMBOL_H = static_cast<int>(static_cast<float>(symbolsRenderer.getSymbolH()) * SCALE);

    std::string formatTime(int seconds);
    std::string formatAmmo(const AmmoInfo& ammoInfo);
    void renderSymbolAndText(Graphics& graphics, HUDSymbolType symbol, const std::string& text,
                             int x, int y);
    void renderHealth(Graphics& graphics, int baseY);
    void renderTime(Graphics& graphics, int screenW, int baseY);
    void renderAmmo(Graphics& graphics, int screenW, int baseY);
    void renderMoney(Graphics& graphics, int screenW, int baseY);

public:
    HUD();

    void updateFromSnapshot(const EntitySnapshot& snap);
    void render(Graphics& graphics);
};

#endif  // HUD_H
