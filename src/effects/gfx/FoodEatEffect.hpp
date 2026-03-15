#pragma once

#include "effects/Effect.hpp"

namespace effects
{

    class FoodEatEffect : public Effect
    {
    public:
        FoodEatEffect(std::vector<game::ColorCell> cells, unsigned int duration);
        void run() override;

    private:
        std::vector<sf::Color> originalColors_;
    };

} // namespace effects
