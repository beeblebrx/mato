#include "render/SnakeCellRenderer.hpp"

#include <array>
#include <cstdlib>

namespace render
{

    namespace
    {
        const std::array<sf::Color, 6> kCelebrationColors = {
            sf::Color{240, 240, 240}, // green (snake cell)
            sf::Color{33, 150, 83},   // dark green
            sf::Color{255, 230, 0},   // yellow
            sf::Color{139, 90, 43},   // brown
            sf::Color{139, 0, 0},     // dark red
            sf::Color{255, 80, 0},    // fiery orange
        };

        const int COLOR_REGEN_INTERVAL = 2;
        const int FIRST_TICK = 999;
    }

    void SnakeCellRenderer::setTicksRemaining(unsigned int ticks)
    {
        ticksRemaining_ = ticks;
    }

    void SnakeCellRenderer::renderCells(sf::RectangleShape cell, const std::vector<game::ColorCell> &cells, game::Phase phase)
    {
        if (phase == game::Phase::LevelPause && (lastTickColorsPicked_ == FIRST_TICK || lastTickColorsPicked_ - ticksRemaining_ >= COLOR_REGEN_INTERVAL))
        {
            celebrationColors_.resize(cells.size());
            for (auto &color : celebrationColors_)
            {
                color = kCelebrationColors[std::rand() % kCelebrationColors.size()];
            }
            lastTickColorsPicked_ = ticksRemaining_;
        }

        for (std::size_t i = 0; i < cells.size(); ++i)
        {
            if (phase == game::Phase::LevelPause && i < celebrationColors_.size())
                cell.setFillColor(celebrationColors_[i]);
            else
                cell.setFillColor(cells[i].color);
            cell.setPosition(
                static_cast<float>(cells[i].position.x * cellSize_ + 1),
                static_cast<float>(statusAreaHeight_ + cells[i].position.y * cellSize_ + 1));
            target_.draw(cell);
        }
    }

} // namespace render
