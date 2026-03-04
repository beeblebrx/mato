#pragma once

#include <array>
#include <vector>

#include "game/Types.hpp"

namespace game::levels
{

    inline constexpr int kLevelWidth = 30;
    inline constexpr int kLevelHeight = 20;

    struct LevelDefinition
    {
        std::array<const char *, kLevelHeight> layoutRows;
        unsigned int foodsToComplete{};
        unsigned int growthPerFood{};
        Position start{};
        Direction startDirection{Direction::Right};
    };

    [[nodiscard]] const std::vector<LevelDefinition> &all();

}
