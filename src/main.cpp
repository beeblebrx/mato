#include <SFML/Graphics.hpp>

#include <chrono>
#include <iomanip>
#include <string>

#include "game/GameState.hpp"
#include "game/LevelData.hpp"
#include "input/KeyboardControl.hpp"
#include "render/RenderAssetsFactory.hpp"
#include "render/GameText.hpp"
#include "render/WindowTitle.hpp"

namespace
{

    constexpr int kGridWidth = game::levels::kLevelWidth;
    constexpr int kGridHeight = game::levels::kLevelHeight;
    constexpr int kCellSize = 24;
    constexpr int kStatusAreaHeight = 64;
    constexpr auto kTickRate = std::chrono::milliseconds(120);

}

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(kGridWidth * kCellSize, kStatusAreaHeight + kGridHeight * kCellSize),
        "Mato",
        sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    game::GameState state(kGridWidth, kGridHeight);
    render::RenderAssetsFactory assets(kGridWidth, kGridHeight, kCellSize, kStatusAreaHeight);
    input::KeyboardControl keyboardControl(state);

    sf::Clock frameClock;
    std::chrono::milliseconds accumulator{0};
    render::WindowTitle windowTitle(window, state);

    windowTitle.update();

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (state.phase() == game::Phase::GameOver || state.phase() == game::Phase::Won)
                {
                    state.restart();
                    accumulator = std::chrono::milliseconds{0};
                    windowTitle.update();
                    continue;
                }
                keyboardControl.handle(event.key.code);
            }
        }

        accumulator += std::chrono::milliseconds(frameClock.restart().asMilliseconds());

        while (accumulator >= kTickRate)
        {
            state.update();
            accumulator -= kTickRate;
            windowTitle.update();
        }

        window.clear(sf::Color(24, 24, 24));
        window.draw(assets.statusAreaBackground());
        window.draw(assets.playfieldBackground());

        for (const game::Position &wall : state.walls())
        {
            sf::RectangleShape wallCell = assets.createWallCell();
            wallCell.setPosition(
                static_cast<float>(wall.x * kCellSize + 1),
                static_cast<float>(kStatusAreaHeight + wall.y * kCellSize + 1));
            window.draw(wallCell);
        }

        if (state.hasFood())
        {
            const game::Position food = state.food();
            sf::RectangleShape foodCell = assets.createFoodCell();
            foodCell.setPosition(
                static_cast<float>(food.x * kCellSize + 1),
                static_cast<float>(kStatusAreaHeight + food.y * kCellSize + 1));
            window.draw(foodCell);
        }

        for (const game::Position &segment : state.snake())
        {
            sf::RectangleShape snakeCell = assets.createSnakeCell();
            snakeCell.setPosition(
                static_cast<float>(segment.x * kCellSize + 1),
                static_cast<float>(kStatusAreaHeight + segment.y * kCellSize + 1));
            window.draw(snakeCell);
        }

        sf::Text overlayText = assets.createOverlayText();
        overlayText.setString(render::GameText::statusText(state));
        window.draw(overlayText);

        window.display();
    }

    return 0;
}
