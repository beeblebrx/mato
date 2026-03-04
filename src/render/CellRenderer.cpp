#include "render/CellRenderer.hpp"

namespace render
{

    CellRenderer::CellRenderer(sf::RenderTarget &target, int cellSize, int statusAreaHeight)
        : cellSize_(cellSize),
          statusAreaHeight_(statusAreaHeight),
          target_(target)
    {
    }

    void CellRenderer::renderCells(sf::RectangleShape cell, const std::vector<game::ColorCell> &cells) const
    {
        for (const game::ColorCell &cellData : cells)
        {
            cell.setFillColor(cellData.color);
            cell.setPosition(
                static_cast<float>(cellData.position.x * cellSize_ + 1),
                static_cast<float>(statusAreaHeight_ + cellData.position.y * cellSize_ + 1));
            target_.draw(cell);
        }
    }

} // namespace render