#pragma once

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

}
