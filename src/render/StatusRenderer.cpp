#include "StatusRenderer.hpp"
#include "GameText.hpp"
#include "../game/Types.hpp"

namespace render
{

    StatusRenderer::StatusRenderer(sf::RectangleShape background,
                                   sf::Text textPrototype,
                                   float statusAreaHeight)
        : background_(std::move(background)), text_(textPrototype), rewardText_(std::move(textPrototype)), statusAreaHeight_(statusAreaHeight)
    {
        dot_.setRadius(6.f);
        dot_.setFillColor(game::kFoodColor);
        dot_.setOrigin(6.f, 6.f);
    }

    void StatusRenderer::render(sf::RenderWindow &window,
                                const game::GameState &state)
    {
        window.draw(background_);

        text_.setString(GameText::statusText(state));
        window.draw(text_);

        if (state.phase() == game::Phase::Running || state.phase() == game::Phase::Paused)
        {
            rewardText_.setString(GameText::rewardText(state));
            rewardText_.setFillColor(GameText::rewardColor(state));
            const float x = background_.getSize().x - rewardText_.getGlobalBounds().width - 10.f;
            const float y = text_.getPosition().y;
            rewardText_.setPosition(x, y);
            window.draw(rewardText_);
        }

        if (!state.isGameOver())
        {
            const sf::FloatRect textBounds = text_.getGlobalBounds();
            const float dotCenterY = textBounds.top + textBounds.height * .85f;
            const float dotStartX = textBounds.left + 130.f;
            const float dotSpacing = 18.f;
            const int remaining = state.foodsRemainingInLevel();
            for (int i = 0; i < remaining; ++i)
            {
                dot_.setPosition(dotStartX + i * dotSpacing, dotCenterY);
                window.draw(dot_);
            }
        }
    }

} // namespace render
