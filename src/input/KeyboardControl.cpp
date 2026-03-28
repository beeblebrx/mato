#include "input/KeyboardControl.hpp"

namespace input
{

    KeyboardControl::KeyboardControl(game::GameState &state)
        : Control(state) {}

    void KeyboardControl::handle(const sf::Keyboard::Key key)
    {
        switch (key)
        {
        case sf::Keyboard::Up:
        case sf::Keyboard::W:
            state_.setDirection(game::Direction::Up);
            break;
        case sf::Keyboard::Down:
        case sf::Keyboard::S:
            state_.setDirection(game::Direction::Down);
            break;
        case sf::Keyboard::Left:
        case sf::Keyboard::A:
            state_.setDirection(game::Direction::Left);
            break;
        case sf::Keyboard::Right:
        case sf::Keyboard::D:
            state_.setDirection(game::Direction::Right);
            break;
#ifdef TESTING
        case sf::Keyboard::Space:
            state_.triggerLevelAdvance();
            break;
#endif
        case sf::Keyboard::P:
            state_.togglePause();
            break;
        default:
            break;
        }
    }

} // namespace input
