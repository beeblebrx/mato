#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <random>
#include <vector>

#include "game/FoodReward.hpp"
#include "game/LevelData.hpp"
#include "game/Types.hpp"

namespace game
{

    enum class Phase
    {
        Running,
        LevelPause,
        GameOver,
        Won
    };

    class GameState
    {
    public:
        GameState(int width, int height);

        void setDirection(Direction direction);
        void update();
        void restart();
        void triggerLevelAdvance();

        std::vector<Event> drainEvents();

        [[nodiscard]] bool isGameOver() const;
        [[nodiscard]] bool isWon() const;
        [[nodiscard]] unsigned int width() const;
        [[nodiscard]] unsigned int height() const;
        [[nodiscard]] unsigned int score() const;
        [[nodiscard]] unsigned int currentLevelNumber() const;
        [[nodiscard]] int foodsRemainingInLevel() const;
        [[nodiscard]] Phase phase() const;
        [[nodiscard]] unsigned int currentFoodReward() const;
        [[nodiscard]] const std::vector<ColorCell> &snake() const;
        [[nodiscard]] const std::vector<ColorCell> &walls() const;
        [[nodiscard]] const std::vector<ColorCell> &foods() const;

    private:
        [[nodiscard]] Direction oppositeDirection(Direction direction) const;
        [[nodiscard]] const std::vector<game::levels::LevelDefinition> &levels() const;
        [[nodiscard]] const game::levels::LevelDefinition &currentLevel() const;
        void doUpdate();
        void loadLevel(std::size_t levelIndex);
        [[nodiscard]] bool isInsideBoard(const Position &position) const;
        [[nodiscard]] bool isWallCell(const Position &position) const;
        [[nodiscard]] bool isOpposite(Direction next) const;
        [[nodiscard]] Position nextHeadPosition(Direction direction) const;
        [[nodiscard]] bool hitsWall(const Position &position) const;
        [[nodiscard]] bool hitsSelf(const Position &position) const;

        unsigned int boardWidth_{};
        unsigned int boardHeight_{};

        std::vector<ColorCell> snake_;
        Direction direction_{Direction::Right};
        std::optional<Direction> pendingDirection_;

        std::vector<ColorCell> foods_;

        unsigned int score_{};
        unsigned short pendingGrowth_{};
        unsigned int foodsEatenInLevel_{};
        std::size_t currentLevelIndex_{};
        unsigned int tick_{};
        unsigned int levelPauseTicksRemaining_{};
        bool levelAdvancePending_{};
        Phase phase_{Phase::Running};

        std::vector<bool> wallMask_;
        std::vector<ColorCell> wallCells_;

        std::mt19937 randomEngine_;
        FoodReward foodReward_;
        std::vector<Event> pendingEvents_;
    };

} // namespace game
