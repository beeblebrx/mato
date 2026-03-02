#pragma once
#include <string>
#include "game/GameState.hpp"

namespace render
{
    class GameText
    {
    public:
        static std::string statusText(const game::GameState &state);
        static std::string windowTitle(const game::GameState &state);
    };
}