#include "render/WindowTitle.hpp"
#include "render/GameText.hpp"

namespace render
{

    WindowTitle::WindowTitle(sf::RenderWindow &window, const game::GameState &state)
        : window_(window), state_(state) {}

    void WindowTitle::update()
    {
        window_.setTitle(GameText::windowTitle(state_));
    }

}