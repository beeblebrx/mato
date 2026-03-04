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

    private:
        unsigned int ticksRemaining_ = 0;
        std::vector<sf::Color> celebrationColors_;
        unsigned int lastTickColorsPicked_ = 999;
    };

} // namespace render
