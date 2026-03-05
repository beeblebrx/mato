#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "game/GameState.hpp"
#include "render/CellRenderer.hpp"

namespace render
{

    class SnakeCellRenderer : public CellRenderer
    {
    public:
        using CellRenderer::CellRenderer;

        void setTicksRemaining(unsigned int ticks);
        void renderCells(sf::RectangleShape cell, const std::vector<game::ColorCell> &cells, game::Phase phase);
        void onGameOverTick();
        bool isGameOverAnimating() const;

    private:
        static constexpr unsigned int FIRST_TICK = 999;
        static constexpr unsigned int GAME_OVER_EFFECT_TICKS = 20;
        unsigned int ticksRemaining_ = 0;
        unsigned int gameOverTick_ = 0;
        std::vector<sf::Color> celebrationColors_;
        unsigned int lastTickColorsPicked_ = FIRST_TICK;
    };

} // namespace render
