#include "GameText.hpp"
#include "game/GameState.hpp"

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

    if (state.phase() == game::Phase::Running)
    {
        status += "  Next: " + std::to_string(state.currentFoodReward()) + " pts";
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