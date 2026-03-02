#include <SFML/Graphics.hpp>

#include <chrono>
#include <iomanip>
#include <string>

#include "game/GameState.hpp"
#include "game/LevelData.hpp"
#include "input/KeyboardControl.hpp"
#include "render/RenderAssetsFactory.hpp"

namespace {

constexpr int kGridWidth = game::levels::kLevelWidth;
constexpr int kGridHeight = game::levels::kLevelHeight;
constexpr int kCellSize = 24;
constexpr int kStatusAreaHeight = 64;
constexpr auto kTickRate = std::chrono::milliseconds(120);

void updateWindowTitle(sf::RenderWindow& window, const game::GameState& state) {
    std::string title = "Mato - L" + std::to_string(state.currentLevelNumber()) + " - Score: " + std::to_string(state.score());
    if (state.isGameOver()) {
        title += " - Game Over";
    } else if (state.isWon()) {
        title += " - You Won";
    }
    window.setTitle(title);
}

std::string statusText(const game::GameState& state) {
    std::string status = "Level " + std::to_string(state.currentLevelNumber()) + "  Score: " + std::to_string(state.score());

    if (state.phase() == game::Phase::Running) {
        status += "  Foods left: " + std::to_string(state.foodsRemainingInLevel());
    } else if (state.phase() == game::Phase::LevelPause) {
        status += "\nLevel complete!";
    } else if (state.phase() == game::Phase::GameOver) {
        status += "\nGame Over - Press any key to restart";
    } else if (state.phase() == game::Phase::Won) {
        status += "\nYou won the game! - Press any key to restart";
    }

    return status;
}

}  // namespace

int main() {
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

    updateWindowTitle(window, state);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (state.phase() == game::Phase::GameOver || state.phase() == game::Phase::Won) {
                    state.restart();
                    accumulator = std::chrono::milliseconds{0};
                    updateWindowTitle(window, state);
                    continue;
                }
                keyboardControl.handle(event.key.code);
            }
        }

        accumulator += std::chrono::milliseconds(frameClock.restart().asMilliseconds());

        while (accumulator >= kTickRate) {
            state.update();
            accumulator -= kTickRate;
            updateWindowTitle(window, state);
        }

        window.clear(sf::Color(24, 24, 24));
        window.draw(assets.statusAreaBackground());
        window.draw(assets.playfieldBackground());

        for (const game::Position& wall : state.walls()) {
            sf::RectangleShape wallCell = assets.createWallCell();
            wallCell.setPosition(
                static_cast<float>(wall.x * kCellSize + 1),
                static_cast<float>(kStatusAreaHeight + wall.y * kCellSize + 1));
            window.draw(wallCell);
        }

        if (state.hasFood()) {
            const game::Position food = state.food();
            sf::RectangleShape foodCell = assets.createFoodCell();
            foodCell.setPosition(
                static_cast<float>(food.x * kCellSize + 1),
                static_cast<float>(kStatusAreaHeight + food.y * kCellSize + 1));
            window.draw(foodCell);
        }

        for (const game::Position& segment : state.snake()) {
            sf::RectangleShape snakeCell = assets.createSnakeCell();
            snakeCell.setPosition(
                static_cast<float>(segment.x * kCellSize + 1),
                static_cast<float>(kStatusAreaHeight + segment.y * kCellSize + 1));
            window.draw(snakeCell);
        }

        sf::Text overlayText = assets.createOverlayText();
        overlayText.setString(statusText(state));
        window.draw(overlayText);

        window.display();
    }

    return 0;
}
