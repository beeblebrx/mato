#pragma once

#include <SFML/Window/Keyboard.hpp>

#include "input/Control.hpp"

namespace input
{

    class KeyboardControl : public Control
    {
    public:
        explicit KeyboardControl(game::GameState &state);
        void handle(const sf::Keyboard::Key key) override;
    };

}
