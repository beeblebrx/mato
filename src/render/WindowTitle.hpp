#pragma once

#include <SFML/Graphics.hpp>
#include "game/GameState.hpp"

namespace render
{
    class WindowTitle
    {
    public:
        explicit WindowTitle(sf::RenderWindow &window, const game::GameState &state);
        void update();

    private:
        sf::RenderWindow &window_;
        const game::GameState &state_;
    };
}