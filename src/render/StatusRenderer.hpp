#pragma once
#include <SFML/Graphics.hpp>
#include "../game/GameState.hpp"

namespace render {

class StatusRenderer {
public:
    StatusRenderer(sf::RectangleShape background, sf::Text textPrototype,
                   float statusAreaHeight);
    void render(sf::RenderWindow& window, const game::GameState& state);

private:
    sf::RectangleShape background_;
    sf::Text text_;
    sf::CircleShape dot_;
    float statusAreaHeight_;
};

} // namespace render
