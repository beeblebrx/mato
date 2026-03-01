#include <SFML/Graphics.hpp>

#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

#include "game/GameState.hpp"
#include "game/LevelData.hpp"

namespace {

constexpr int kGridWidth = game::levels::kLevelWidth;
constexpr int kGridHeight = game::levels::kLevelHeight;
constexpr int kCellSize = 24;
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
    std::ostringstream stream;
    stream << "Level " << state.currentLevelNumber() << "  Score: " << state.score();

    if (state.phase() == game::Phase::Running) {
        stream << "  Foods left: " << state.foodsRemainingInLevel();
    } else if (state.phase() == game::Phase::LevelPause) {
        const double seconds = (state.levelPauseTicksRemaining() * kTickRate.count()) / 1000.0;
        stream << "\nLevel complete! Next level in " << std::fixed << std::setprecision(1) << seconds << "s";
    } else if (state.phase() == game::Phase::GameOver) {
        stream << "\nGame Over";
    } else if (state.phase() == game::Phase::Won) {
        stream << "\nYou won the game!";
    }

    return stream.str();
}

void handleDirectionInput(game::GameState& state, const sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::Up:
        case sf::Keyboard::W:
            state.setDirection(game::Direction::Up);
            break;
        case sf::Keyboard::Down:
        case sf::Keyboard::S:
            state.setDirection(game::Direction::Down);
            break;
        case sf::Keyboard::Left:
        case sf::Keyboard::A:
            state.setDirection(game::Direction::Left);
            break;
        case sf::Keyboard::Right:
        case sf::Keyboard::D:
            state.setDirection(game::Direction::Right);
            break;
        default:
            break;
    }
}

}  // namespace

int main() {
    sf::RenderWindow window(
        sf::VideoMode(kGridWidth * kCellSize, kGridHeight * kCellSize),
        "Mato",
        sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    game::GameState state(kGridWidth, kGridHeight);

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/Comfortaa-Regular.otf")) {
        throw std::runtime_error("Failed to load font asset: assets/fonts/Comfortaa-Regular.otf");
    }

    sf::RectangleShape snakeCell(sf::Vector2f(static_cast<float>(kCellSize - 2), static_cast<float>(kCellSize - 2)));
    snakeCell.setFillColor(sf::Color(46, 204, 113));

    sf::RectangleShape foodCell(sf::Vector2f(static_cast<float>(kCellSize - 2), static_cast<float>(kCellSize - 2)));
    foodCell.setFillColor(sf::Color(231, 76, 60));

    sf::RectangleShape wallCell(sf::Vector2f(static_cast<float>(kCellSize - 2), static_cast<float>(kCellSize - 2)));
    wallCell.setFillColor(sf::Color(127, 140, 141));

    sf::Text overlayText;
    overlayText.setFont(font);
    overlayText.setCharacterSize(20);
    overlayText.setFillColor(sf::Color(236, 240, 241));
    overlayText.setPosition(10.f, 6.f);

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
                handleDirectionInput(state, event.key.code);
            }
        }

        accumulator += std::chrono::milliseconds(frameClock.restart().asMilliseconds());

        while (accumulator >= kTickRate) {
            state.update();
            accumulator -= kTickRate;
            updateWindowTitle(window, state);
        }

        window.clear(sf::Color(24, 24, 24));

        for (const game::Position& wall : state.walls()) {
            wallCell.setPosition(static_cast<float>(wall.x * kCellSize + 1), static_cast<float>(wall.y * kCellSize + 1));
            window.draw(wallCell);
        }

        if (state.hasFood()) {
            const game::Position food = state.food();
            foodCell.setPosition(static_cast<float>(food.x * kCellSize + 1), static_cast<float>(food.y * kCellSize + 1));
            window.draw(foodCell);
        }

        for (const game::Position& segment : state.snake()) {
            snakeCell.setPosition(static_cast<float>(segment.x * kCellSize + 1), static_cast<float>(segment.y * kCellSize + 1));
            window.draw(snakeCell);
        }

        overlayText.setString(statusText(state));
        window.draw(overlayText);

        window.display();
    }

    return 0;
}
