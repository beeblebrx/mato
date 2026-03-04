#include "render/RenderAssetsFactory.hpp"

#include <stdexcept>

namespace render
{

    RenderAssetsFactory::RenderAssetsFactory(int gridWidth, int gridHeight, int cellSize, int statusAreaHeight)
        : statusAreaBackground_(sf::Vector2f(static_cast<float>(gridWidth * cellSize), static_cast<float>(statusAreaHeight))),
          playfieldBackground_(sf::Vector2f(static_cast<float>(gridWidth * cellSize), static_cast<float>(gridHeight * cellSize))),
          cellPrototype_(sf::Vector2f(static_cast<float>(cellSize - 2), static_cast<float>(cellSize - 2)))
    {
        if (!font_.loadFromFile("assets/fonts/Comfortaa-Regular.otf"))
        {
            throw std::runtime_error("Failed to load font asset: assets/fonts/Comfortaa-Regular.otf");
        }

        statusAreaBackground_.setFillColor(sf::Color(44, 62, 80));
        playfieldBackground_.setFillColor(sf::Color(24, 24, 24));
        playfieldBackground_.setPosition(0.f, static_cast<float>(statusAreaHeight));

        overlayTextPrototype_.setFont(font_);
        overlayTextPrototype_.setCharacterSize(20);
        overlayTextPrototype_.setFillColor(sf::Color(236, 240, 241));
        overlayTextPrototype_.setPosition(10.f, 8.f);
    }

    const sf::RectangleShape &RenderAssetsFactory::statusAreaBackground() const
    {
        return statusAreaBackground_;
    }

    const sf::RectangleShape &RenderAssetsFactory::playfieldBackground() const
    {
        return playfieldBackground_;
    }

    sf::RectangleShape RenderAssetsFactory::createCellPrototype() const
    {
        return cellPrototype_;
    }

    sf::Text RenderAssetsFactory::createOverlayText() const
    {
        return overlayTextPrototype_;
    }

} // namespace render
