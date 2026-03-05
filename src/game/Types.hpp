#pragma once

#include <SFML/Graphics/Color.hpp>

namespace game
{

    enum class Direction
    {
        Up,
        Down,
        Left,
        Right
    };

    struct Position
    {
        int x{};
        int y{};

        bool operator==(const Position &other) const
        {
            return x == other.x && y == other.y;
        }
    };

    struct ColorCell
    {
        Position position{};
        sf::Color color{};
    };

    inline const sf::Color kSnakeHeadColor{205, 127, 50};
    // Shades of green for the snake body, will be cycled through to create a gradient effect
    inline const sf::Color kSnakeBodyShade1{46, 204, 113};
    inline const sf::Color kSnakeBodyShade2{39, 174, 96};
    inline const sf::Color kSnakeBodyShade3{33, 150, 83};
    inline const sf::Color kFoodColor{231, 76, 60};
    inline const sf::Color kWallColor{127, 140, 141};
    inline const sf::Color kSnakeEatFlashColor{144, 238, 144};

}
