#pragma once

#include <SFML/Window/Keyboard.hpp>

#include "game/GameState.hpp"

namespace input
{

    class Control
    {
    public:
        explicit Control(game::GameState &state)
            : state_(state) {}
        virtual ~Control() = default;
        virtual void handle(const sf::Keyboard::Key key) = 0;

    protected:
        game::GameState &state_;
    };

}
