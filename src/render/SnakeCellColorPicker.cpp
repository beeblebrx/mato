#include "render/SnakeCellColorPicker.hpp"

#include <array>
#include <cstdlib>

namespace render
{

    namespace
    {
        const std::array<sf::Color, 6> kCelebrationColors = {
            sf::Color{ 46, 204, 113},  // green (snake cell)
            sf::Color{ 33, 150,  83},  // dark green
            sf::Color{255, 230,   0},  // yellow
            sf::Color{139,  90,  43},  // brown
            sf::Color{139,   0,   0},  // dark red
            sf::Color{255,  80,   0},  // fiery orange
        };
    }

    sf::Color SnakeCellColorPicker::pick(const game::ColorCell &cell, game::Phase phase) const
    {
        if (phase == game::Phase::LevelPause)
        {
            return kCelebrationColors[std::rand() % kCelebrationColors.size()];
        }
        return cell.color;
    }

} // namespace render
