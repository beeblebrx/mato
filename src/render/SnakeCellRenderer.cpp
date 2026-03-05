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
    }

    void SnakeCellRenderer::setTicksRemaining(unsigned int ticks)
    {
        ticksRemaining_ = ticks;
    }

    void SnakeCellRenderer::setFoodEatEffectTicks(unsigned int ticks)
    {
        foodEatEffectTicks_ = ticks;
    }

    void SnakeCellRenderer::onGameOverTick()
    {
        if (gameOverTick_ <= GAME_OVER_EFFECT_TICKS)
            ++gameOverTick_;
    }

    bool SnakeCellRenderer::isGameOverAnimating() const
    {
        return gameOverTick_ <= GAME_OVER_EFFECT_TICKS;
    }

    void SnakeCellRenderer::renderCells(sf::RectangleShape cell, const std::vector<game::ColorCell> &cells, game::Phase phase)
    {
        if (phase != game::Phase::GameOver)
            gameOverTick_ = 0;

        if (phase == game::Phase::LevelPause && (lastTickColorsPicked_ == FIRST_TICK || lastTickColorsPicked_ - ticksRemaining_ >= COLOR_REGEN_INTERVAL))
        {
            celebrationColors_.resize(cells.size());
            for (auto &color : celebrationColors_)
            {
                color = kCelebrationColors[std::rand() % kCelebrationColors.size()];
            }
            lastTickColorsPicked_ = ticksRemaining_;
        }

        if (phase == game::Phase::Running && foodEatEffectTicks_ > 0)
        {
            if (preEatColors_.size() != cells.size())
            {
                preEatColors_.resize(cells.size());
                for (std::size_t i = 0; i < cells.size(); ++i)
                    preEatColors_[i] = cells[i].color;
            }
        }
        else
        {
            preEatColors_.clear();
        }

        for (std::size_t i = 0; i < cells.size(); ++i)
        {
            if (phase == game::Phase::GameOver)
            {
                unsigned int t = std::min(gameOverTick_, GAME_OVER_EFFECT_TICKS);
                sf::Uint8 red = static_cast<sf::Uint8>(255 - (235 * t / GAME_OVER_EFFECT_TICKS));
                cell.setFillColor(sf::Color(red, 0, 0));
            }
            else if (phase == game::Phase::LevelPause && i < celebrationColors_.size())
                cell.setFillColor(celebrationColors_[i]);
            else if (foodEatEffectTicks_ > 0 && i < preEatColors_.size())
            {
                float blend = foodEatEffectTicks_ / 3.0f;
                const sf::Color &orig = preEatColors_[i];
                const sf::Color &flash = game::kSnakeEatFlashColor;
                cell.setFillColor(sf::Color(
                    static_cast<sf::Uint8>(orig.r + blend * (flash.r - orig.r)),
                    static_cast<sf::Uint8>(orig.g + blend * (flash.g - orig.g)),
                    static_cast<sf::Uint8>(orig.b + blend * (flash.b - orig.b))));
            }
            else
                cell.setFillColor(cells[i].color);
            cell.setPosition(
                static_cast<float>(cells[i].position.x * cellSize_ + 1),
                static_cast<float>(statusAreaHeight_ + cells[i].position.y * cellSize_ + 1));
            target_.draw(cell);
        }
    }

} // namespace render
