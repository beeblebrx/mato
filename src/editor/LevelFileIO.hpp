#pragma once

#include <array>
#include <string>

namespace editor
{

    constexpr int kLevelWidth = 30;
    constexpr int kLevelHeight = 20;

    struct LevelData
    {
        std::array<std::string, kLevelHeight> rows;
        int foodsToComplete = 1;
        int growthPerFood = 1;
        int startX = 0;
        int startY = 0;
        std::string startDirection = "right";
    };

    // Returns default LevelData if file doesn't exist.
    // Throws std::runtime_error on parse errors.
    LevelData loadLevel(const std::string &filepath);
    void saveLevel(const std::string &filepath, const LevelData &data);

}
