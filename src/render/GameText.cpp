#include "GameText.hpp"
#include "game/GameState.hpp"
#include <SFML/Graphics/Color.hpp>

std::string render::GameText::windowTitle(const game::GameState &state)
{
    std::string title = "Mato - L" + std::to_string(state.currentLevelNumber()) + " - Score: " + std::to_string(state.score());
    if (state.isGameOver())
    {
        title += " - Game Over";
    }
    else if (state.isWon())
    {
        title += " - You Won";
    }
    return title;
}

std::string render::GameText::statusText(const game::GameState &state)
{
    std::string status = "Level " + std::to_string(state.currentLevelNumber()) + "  Score: " + std::to_string(state.score());

    if (state.phase() == game::Phase::Running || state.phase() == game::Phase::Paused)
    {
        status += "\nFoods left:";
    }
    else if (state.phase() == game::Phase::LevelPause)
    {
        status += "\nLevel complete!";
    }
    else if (state.phase() == game::Phase::GameOver)
    {
        status += "\nGame Over - Press any key to restart";
    }
    else if (state.phase() == game::Phase::Won)
    {
        status += "\nYou won the game! - Press any key to restart";
    }

    return status;
}

std::string render::GameText::rewardText(const game::GameState &state)
{
    return "Next: " + std::to_string(state.currentFoodReward()) + " pts";
}

sf::Color render::GameText::rewardColor(const game::GameState &state)
{
    const int reward = state.currentFoodReward();
    const float r = static_cast<float>(reward);

    auto lerp = [](sf::Color a, sf::Color b, float t) -> sf::Color
    {
        return sf::Color(
            static_cast<sf::Uint8>(a.r + (b.r - a.r) * t),
            static_cast<sf::Uint8>(a.g + (b.g - a.g) * t),
            static_cast<sf::Uint8>(a.b + (b.b - a.b) * t));
    };

    const sf::Color orange(255, 180, 0);
    const sf::Color white(255, 255, 255);
    const sf::Color gray(130, 130, 130);

    if (r >= 500.f)
        return lerp(white, orange, (r - 500.f) / 500.f);
    return lerp(gray, white, (r - 10.f) / 490.f);
}