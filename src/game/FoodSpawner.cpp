#include "game/FoodSpawner.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace game
{

    ColorCell FoodSpawner::spawn(
        std::mt19937 &randomEngine,
        int width,
        int height,
        const std::vector<ColorCell> &snake,
        const std::vector<bool> &blockedCells)
    {
        std::vector<Position> freeCells;
        freeCells.reserve(static_cast<size_t>(width * height));

        const bool hasBlockedCells = !blockedCells.empty();

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                const Position cell{x, y};
                const size_t index = static_cast<size_t>(y * width + x);
                const bool blocked = hasBlockedCells && index < blockedCells.size() && blockedCells[index];
                const auto occupied = std::find_if(snake.begin(), snake.end(),
                                                   [&cell](const ColorCell &snakeCell)
                                                   {
                                                       return snakeCell.position == cell;
                                                   }) != snake.end();
                if (!occupied && !blocked)
                {
                    freeCells.push_back(cell);
                }
            }
        }

        if (freeCells.empty())
        {
            throw std::runtime_error("No free cells available for spawning food");
        }

        std::uniform_int_distribution<size_t> distribution(0, freeCells.size() - 1);
        return {freeCells[distribution(randomEngine)], kFoodColor};
    }

}
