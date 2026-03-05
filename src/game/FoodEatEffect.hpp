#pragma once

#include "game/Effect.hpp"

namespace game
{

    class FoodEatEffect : public Effect
    {
    public:
        FoodEatEffect(std::vector<ColorCell> cells, unsigned int duration);
        void run() override;

    private:
        std::vector<sf::Color> originalColors_;
    };

} // namespace game
