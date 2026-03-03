#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "game/Types.hpp"

namespace render
{

    class CellRenderer
    {
    public:
        CellRenderer(sf::RenderTarget &target, int cellSize, int statusAreaHeight);

        void renderCells(sf::RectangleShape cell, const std::vector<game::Position> &positions) const;

    private:
        int cellSize_;
        int statusAreaHeight_;
        sf::RenderTarget &target_;
    };

} // namespace render