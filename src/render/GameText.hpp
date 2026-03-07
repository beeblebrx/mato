#pragma once
#include <string>
#include <SFML/Graphics/Color.hpp>
#include "game/GameState.hpp"

namespace render
{
    class GameText
    {
    public:
        static std::string statusText(const game::GameState &state);
        static std::string windowTitle(const game::GameState &state);
        static std::string rewardText(const game::GameState &state);
        static sf::Color rewardColor(const game::GameState &state);
    };
}