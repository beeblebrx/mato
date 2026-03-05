#include "game/GameState.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>

#include "game/CelebrationEffect.hpp"
#include "game/DeathEffect.hpp"
#include "game/FoodEatEffect.hpp"
#include "game/FoodSpawner.hpp"

namespace game
{

    namespace
    {

        constexpr int kInitialLength = 3;
        constexpr int kLevelPauseTicks = 25;
        const sf::Color kSnakeBodyCycle[] = {
            kSnakeBodyShade1,
            kSnakeBodyShade2,
            kSnakeBodyShade3,
            kSnakeBodyShade2,
            kSnakeBodyShade1};

        bool hasPosition(const ColorCell &cell, const Position &position)
        {
            return cell.position == position;
        }

    }

    GameState::GameState(int width, int height)
        : boardWidth_(width),
          boardHeight_(height),
          randomEngine_(std::random_device{}()),
          tick_(0)
    {
        if (boardWidth_ != levels::kLevelWidth || boardHeight_ != levels::kLevelHeight)
        {
            throw std::invalid_argument("Game dimensions must match level dimensions");
        }
        if (levels().empty())
        {
            throw std::runtime_error("No level definitions configured");
        }

        restart();
    }

    void GameState::setDirection(Direction direction)
    {
        pendingDirection_ = direction;
    }

    void GameState::update()
    {
        doUpdate();
        effectEngine_.run(snake_);
    }

    void GameState::doUpdate()
    {
        if (phase_ == Phase::GameOver || phase_ == Phase::Won)
        {
            return;
        }

        if (phase_ == Phase::LevelPause)
        {
            if (levelPauseTicksRemaining_ > 0)
            {
                --levelPauseTicksRemaining_;
            }
            if (levelPauseTicksRemaining_ == 0)
            {
                effectEngine_.clear();
                if (currentLevelIndex_ + 1 >= levels().size())
                {
                    phase_ = Phase::Won;
                    foods_.clear();
                }
                else
                {
                    ++currentLevelIndex_;
                    loadLevel(currentLevelIndex_);
                    phase_ = Phase::Running;
                }
            }
            return;
        }

        if (levelAdvancePending_)
        {
            levelAdvancePending_ = false;
            phase_ = Phase::LevelPause;
            levelPauseTicksRemaining_ = kLevelPauseTicks;
            effectEngine_.add(std::make_unique<CelebrationEffect>(snake_, kLevelPauseTicks));
            return;
        }

        if (pendingDirection_.has_value() && !isOpposite(*pendingDirection_))
        {
            direction_ = *pendingDirection_;
        }
        pendingDirection_.reset();

        const Position nextHead = nextHeadPosition(direction_);

        if (hitsWall(nextHead) || hitsSelf(nextHead))
        {
            phase_ = Phase::GameOver;
            foods_.clear();
            effectEngine_.add(std::make_unique<DeathEffect>(snake_, 20));
            return;
        }

        snake_.insert(snake_.begin(), ColorCell{nextHead, kSnakeBodyCycle[tick_ % std::size(kSnakeBodyCycle)]});

        const auto eatenFood = std::find_if(foods_.begin(), foods_.end(),
                                            [&nextHead](const ColorCell &cell)
                                            {
                                                return hasPosition(cell, nextHead);
                                            });
        if (eatenFood != foods_.end())
        {
            foods_.erase(eatenFood);
            ++score_;
            ++foodsEatenInLevel_;
            pendingGrowth_ += currentLevel().growthPerFood;
            effectEngine_.add(std::make_unique<FoodEatEffect>(snake_, 3));

            if (foodsEatenInLevel_ >= currentLevel().foodsToComplete)
            {
                levelAdvancePending_ = true;
                foods_.clear();
            }
            else
            {
                foods_.push_back(FoodSpawner::spawn(randomEngine_, boardWidth_, boardHeight_, snake_, wallMask_));
            }
        }

        if (pendingGrowth_ > 0)
        {
            --pendingGrowth_;
        }
        else
        {
            snake_.pop_back();
        }

        ++tick_;
    }

    void GameState::restart()
    {
        score_ = 0;
        currentLevelIndex_ = 0;
        levelPauseTicksRemaining_ = 0;
        levelAdvancePending_ = false;
        effectEngine_.clear();
        phase_ = Phase::Running;
        loadLevel(currentLevelIndex_);
    }

    bool GameState::isGameOver() const
    {
        return phase_ == Phase::GameOver;
    }

    bool GameState::isWon() const
    {
        return phase_ == Phase::Won;
    }

    unsigned int GameState::width() const
    {
        return boardWidth_;
    }

    unsigned int GameState::height() const
    {
        return boardHeight_;
    }

    unsigned int GameState::score() const
    {
        return score_;
    }

    unsigned int GameState::currentLevelNumber() const
    {
        return static_cast<unsigned int>(currentLevelIndex_ + 1);
    }

    int GameState::foodsRemainingInLevel() const
    {
        const int remaining = currentLevel().foodsToComplete - foodsEatenInLevel_;
        return remaining > 0 ? remaining : 0;
    }

    Phase GameState::phase() const
    {
        return phase_;
    }

    const std::vector<ColorCell> &GameState::effectSnake() const
    {
        if (effectEngine_.hasEffects())
            return effectEngine_.cells();
        return snake_;
    }

    const EffectEngine &GameState::effectEngine() const
    {
        return effectEngine_;
    }

    const std::vector<ColorCell> &GameState::snake() const
    {
        return snake_;
    }

    const std::vector<ColorCell> &GameState::walls() const
    {
        return wallCells_;
    }

    const std::vector<ColorCell> &GameState::foods() const
    {
        return foods_;
    }

    Direction GameState::oppositeDirection(Direction direction) const
    {
        switch (direction)
        {
        case Direction::Up:
            return Direction::Down;
        case Direction::Down:
            return Direction::Up;
        case Direction::Left:
            return Direction::Right;
        case Direction::Right:
            return Direction::Left;
        }
        return Direction::Right;
    }

    const std::vector<levels::LevelDefinition> &GameState::levels() const
    {
        return levels::all();
    }

    const levels::LevelDefinition &GameState::currentLevel() const
    {
        return levels()[currentLevelIndex_];
    }

    void GameState::loadLevel(std::size_t levelIndex)
    {
        const levels::LevelDefinition &level = levels()[levelIndex];

        wallMask_.assign(static_cast<std::size_t>(boardWidth_ * boardHeight_), false);
        wallCells_.clear();

        for (int y = 0; y < boardHeight_; ++y)
        {
            const char *row = level.layoutRows[static_cast<std::size_t>(y)];
            const std::string rowString{row};
            if (static_cast<int>(rowString.size()) != boardWidth_)
            {
                throw std::runtime_error("Level row width does not match board width");
            }
            for (int x = 0; x < boardWidth_; ++x)
            {
                const char cell = rowString[static_cast<std::size_t>(x)];
                if (cell != '.' && cell != '#')
                {
                    throw std::runtime_error("Unsupported level cell character");
                }
                if (cell == '#')
                {
                    const std::size_t index = static_cast<std::size_t>(y * boardWidth_ + x);
                    wallMask_[index] = true;
                    wallCells_.push_back({{x, y}, kWallColor});
                }
            }
        }

        if (!isInsideBoard(level.start) || isWallCell(level.start))
        {
            throw std::runtime_error("Invalid level start position");
        }

        direction_ = level.startDirection;
        pendingDirection_.reset();

        snake_.clear();
        snake_.reserve(kInitialLength);
        snake_.push_back({level.start, kSnakeBodyCycle[0]});

        Position segment = level.start;
        const Direction tailDirection = oppositeDirection(direction_);
        for (int i = 1; i < kInitialLength; ++i)
        {
            switch (tailDirection)
            {
            case Direction::Up:
                --segment.y;
                break;
            case Direction::Down:
                ++segment.y;
                break;
            case Direction::Left:
                --segment.x;
                break;
            case Direction::Right:
                ++segment.x;
                break;
            }

            if (!isInsideBoard(segment) || isWallCell(segment))
            {
                throw std::runtime_error("Invalid snake spawn for level");
            }

            snake_.push_back({segment, kSnakeBodyShade1});
        }

        foodsEatenInLevel_ = 0;
        pendingGrowth_ = 0;
        levelAdvancePending_ = false;
        levelPauseTicksRemaining_ = 0;

        foods_.clear();
        foods_.push_back(FoodSpawner::spawn(randomEngine_, boardWidth_, boardHeight_, snake_, wallMask_));
    }

    bool GameState::isInsideBoard(const Position &position) const
    {
        return position.x >= 0 && position.y >= 0 && position.x < boardWidth_ && position.y < boardHeight_;
    }

    bool GameState::isWallCell(const Position &position) const
    {
        if (!isInsideBoard(position))
        {
            return false;
        }
        const std::size_t index = static_cast<std::size_t>(position.y * boardWidth_ + position.x);
        return wallMask_[index];
    }

    bool GameState::isOpposite(Direction next) const
    {
        return (direction_ == Direction::Up && next == Direction::Down) ||
               (direction_ == Direction::Down && next == Direction::Up) ||
               (direction_ == Direction::Left && next == Direction::Right) ||
               (direction_ == Direction::Right && next == Direction::Left);
    }

    Position GameState::nextHeadPosition(Direction direction) const
    {
        Position next = snake_.front().position;

        switch (direction)
        {
        case Direction::Up:
            --next.y;
            break;
        case Direction::Down:
            ++next.y;
            break;
        case Direction::Left:
            --next.x;
            break;
        case Direction::Right:
            ++next.x;
            break;
        }

        return next;
    }

    bool GameState::hitsWall(const Position &position) const
    {
        return !isInsideBoard(position) || isWallCell(position);
    }

    bool GameState::hitsSelf(const Position &position) const
    {
        for (const ColorCell &segment : snake_)
        {
            if (hasPosition(segment, position))
            {
                return true;
            }
        }
        return false;
    }

} // namespace game
