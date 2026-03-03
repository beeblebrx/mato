#include "render/CellRenderer.hpp"

namespace render
{

    CellRenderer::CellRenderer(sf::RenderTarget &target, int cellSize, int statusAreaHeight)
        : cellSize_(cellSize),
          statusAreaHeight_(statusAreaHeight),
          target_(target)
    {
    }

    void CellRenderer::renderCells(sf::RectangleShape cell, const std::vector<game::Position> &positions) const
    {
        for (const game::Position &position : positions)
        {
            cell.setPosition(
                static_cast<float>(position.x * cellSize_ + 1),
                static_cast<float>(statusAreaHeight_ + position.y * cellSize_ + 1));
            target_.draw(cell);
        }
    }

} // namespace render