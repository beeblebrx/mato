#include <SFML/Graphics.hpp>

#include <chrono>

#include "game/GameState.hpp"
#include "game/LevelData.hpp"
#include "input/KeyboardControl.hpp"
#include "render/CellRenderer.hpp"
#include "render/SnakeCellRenderer.hpp"
#include "render/RenderAssetsFactory.hpp"
#include "render/GameText.hpp"
#include "render/StatusRenderer.hpp"
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
    render::CellRenderer cellRenderer(window, kCellSize, kStatusAreaHeight);
    render::StatusRenderer statusRenderer{
        assets.statusAreaBackground(),
        assets.createOverlayText(),
        static_cast<float>(kStatusAreaHeight)};
    render::SnakeCellRenderer snakeRenderer(window, kCellSize, kStatusAreaHeight);
    input::KeyboardControl keyboardControl(state);

    sf::Clock frameClock;
    std::chrono::milliseconds accumulator{0};
    render::WindowTitle windowTitle(window, state);
    const sf::RectangleShape cellPrototype = assets.createCellPrototype();

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
                if (state.phase() == game::Phase::Won ||
                    (state.phase() == game::Phase::GameOver && !snakeRenderer.isGameOverAnimating()))
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
            if (state.phase() == game::Phase::GameOver)
                snakeRenderer.onGameOverTick();
            accumulator -= kTickRate;
            windowTitle.update();
        }

        window.clear(sf::Color(24, 24, 24));
        window.draw(assets.playfieldBackground());

        // Render walls
        cellRenderer.renderCells(cellPrototype, state.walls());

        // Render food
        cellRenderer.renderCells(cellPrototype, state.foods());

        // Render snake
        snakeRenderer.setTicksRemaining(state.levelPauseTicksRemaining());
        snakeRenderer.setFoodEatEffectTicks(state.foodEatEffectTicksRemaining());
        snakeRenderer.renderCells(cellPrototype, state.snake(), state.phase());

        statusRenderer.render(window, state);

        window.display();
    }

    return 0;
}
