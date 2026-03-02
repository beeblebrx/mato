#pragma once

#include <SFML/Graphics.hpp>

namespace render {

class RenderAssetsFactory {
public:
    RenderAssetsFactory(int gridWidth, int gridHeight, int cellSize, int statusAreaHeight);

    const sf::RectangleShape& statusAreaBackground() const;
    const sf::RectangleShape& playfieldBackground() const;

    sf::RectangleShape createSnakeCell() const;
    sf::RectangleShape createFoodCell() const;
    sf::RectangleShape createWallCell() const;
    sf::Text createOverlayText() const;

private:
    sf::Font font_;
    sf::RectangleShape statusAreaBackground_;
    sf::RectangleShape playfieldBackground_;
    sf::RectangleShape snakeCellPrototype_;
    sf::RectangleShape foodCellPrototype_;
    sf::RectangleShape wallCellPrototype_;
    sf::Text overlayTextPrototype_;
};

}  // namespace render
