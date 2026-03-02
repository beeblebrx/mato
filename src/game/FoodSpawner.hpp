#pragma once

#include <random>
#include <vector>

#include "game/Types.hpp"

namespace game
{

    class FoodSpawner
    {
    public:
        static Position spawn(
            std::mt19937 &randomEngine,
            int width,
            int height,
            const std::vector<Position> &snake,
            const std::vector<bool> &blockedCells);
    };

}
