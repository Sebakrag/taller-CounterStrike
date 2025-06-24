#include "client/include/model/StatisticsPanel.h"

#include "model/Graphics.h"
#include "model/utils/TextureManager.h"

namespace {
constexpr Color WHITE = {255, 255, 255, 255};
constexpr Color SEMI_TRANSPARENT_BLACK = {0, 0, 0, 180};
}  // namespace

StatisticsPanel::StatisticsPanel(Graphics& graphics, const StatsInfo& stats_info,
                                 const std::string& font_filename, const int font_size):
        graphics(graphics) {
    // int screenW, screenH;
    // graphics.getOutputSize(screenW, screenH); //TODO: implementar este metdo.
    const Vec2D winDimension = graphics.getDrawableWindowDimension();
    const int screenW = winDimension.getX();
    const int screenH = winDimension.getY();

    // Si no recibimos font_size ajustamos por default a ~3.5% del alto de la pantalla.
    const int fontSize = (font_size != 0) ? font_size : static_cast<int>(0.035f * screenH);
    Font font(font_filename, fontSize);

    int lineHeight = static_cast<int>(fontSize * 1.2f);  // línea 20% más alta que la fuente
    int currentY = screenH * 0.15f;

    std::vector<std::string> formattedLines;

    formattedLines.emplace_back("== Terrorist Team ==");
    for (const auto& player: stats_info.terroristStats) {
        formattedLines.emplace_back(player.username + " - Kills: " + std::to_string(player.kills) +
                                    " - Deaths: " + std::to_string(player.deaths) + " - Money: $" +
                                    std::to_string(player.moneyEarned));
    }

    formattedLines.emplace_back(" ");  // Espaciado
    formattedLines.emplace_back("== Counter Team ==");
    for (const auto& player: stats_info.counterStats) {
        formattedLines.emplace_back(player.username + " - Kills: " + std::to_string(player.kills) +
                                    " - Deaths: " + std::to_string(player.deaths) + " - Money: $" +
                                    std::to_string(player.moneyEarned));
    }

    // Crear las texturas y posiciones una vez
    for (const auto& line: formattedLines) {
        auto texture = TextureManager::createTextTexture(line, font, WHITE);
        int texW = texture->GetWidth();
        int texH = texture->GetHeight();

        Rect dst((screenW - texW) / 2, currentY, texW, texH);  // centrado
        lines.push_back({std::move(texture), dst});

        currentY += lineHeight;
    }

    backgroundRect = Rect(0, 0, screenW, screenH);
}

void StatisticsPanel::render() {
    graphics.fillRectBlended(backgroundRect, SEMI_TRANSPARENT_BLACK);

    for (const auto& line: lines) {
        graphics.draw(*line.texture, std::nullopt, line.dstRect);
    }
}
