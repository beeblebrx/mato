#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <random>
#include <vector>

#include "game/LevelData.hpp"
#include "game/Types.hpp"

namespace game {

enum class Phase {
    Running,
    LevelPause,
    GameOver,
    Won
};

class GameState {
public:
    GameState(int width, int height);

    void setDirection(Direction direction);
    void update();
    void restart();

    [[nodiscard]] bool isGameOver() const;
    [[nodiscard]] bool isWon() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] int score() const;
    [[nodiscard]] int currentLevelNumber() const;
    [[nodiscard]] int foodsRemainingInLevel() const;
    [[nodiscard]] Phase phase() const;
    [[nodiscard]] int levelPauseTicksRemaining() const;
    [[nodiscard]] bool hasFood() const;
    [[nodiscard]] const std::vector<Position>& snake() const;
    [[nodiscard]] const std::vector<Position>& walls() const;
    [[nodiscard]] Position food() const;

private:
    [[nodiscard]] Direction oppositeDirection(Direction direction) const;
    [[nodiscard]] const std::vector<game::levels::LevelDefinition>& levels() const;
    [[nodiscard]] const game::levels::LevelDefinition& currentLevel() const;
    void loadLevel(std::size_t levelIndex);
    [[nodiscard]] bool isInsideBoard(const Position& position) const;
    [[nodiscard]] bool isWallCell(const Position& position) const;
    [[nodiscard]] bool isOpposite(Direction next) const;
    [[nodiscard]] Position nextHeadPosition(Direction direction) const;
    [[nodiscard]] bool hitsWall(const Position& position) const;
    [[nodiscard]] bool hitsSelf(const Position& position) const;

    int boardWidth_{};
    int boardHeight_{};

    std::vector<Position> snake_;
    Direction direction_{Direction::Right};
    std::optional<Direction> pendingDirection_;

    Position food_{};
    bool hasFood_{};

    int score_{};
    int pendingGrowth_{};
    int foodsEatenInLevel_{};
    std::size_t currentLevelIndex_{};
    int levelPauseTicksRemaining_{};
    bool levelAdvancePending_{};
    Phase phase_{Phase::Running};

    std::vector<bool> wallMask_;
    std::vector<Position> wallCells_;

    std::mt19937 randomEngine_;
};

}  // namespace game
