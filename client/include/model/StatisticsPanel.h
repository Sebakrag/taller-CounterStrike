#ifndef STATISTICS_PANEL_H
#define STATISTICS_PANEL_H

#include <memory>
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "common/game_info/stats_info.h"

using SDL2pp::Color;
using SDL2pp::Font;
using SDL2pp::Rect;
using SDL2pp::Texture;

class Graphics;

class StatisticsPanel {
private:
    struct TextLine {
        std::unique_ptr<Texture> texture;
        Rect dstRect;
    };

    // std::vector<TextLine> terroristTexts;
    // std::vector<TextLine> counterTexts;
    // TextLine terroristTitle;
    // TextLine counterTitle;
    Graphics& graphics;
    std::vector<TextLine> lines;
    Rect backgroundRect;

    void generateTeamSection(const std::vector<PlayerStatsInfo>& stats,
                             std::vector<TextLine>& outputTexts, int startY, Font& font,
                             const Color& textColor) const;

public:
    StatisticsPanel(Graphics& graphics, const StatsInfo& stats_info,
                    const std::string& font_filename, int font_size = 0);

    void render();
};

#endif  // STATISTICS_PANEL_H
