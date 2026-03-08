#pragma once

#include "render/effects/Effect.hpp"

namespace render
{

    class FoodEatEffect : public Effect
    {
    public:
        FoodEatEffect(std::vector<game::ColorCell> cells, unsigned int duration);
        void run() override;

    private:
        std::vector<sf::Color> originalColors_;
    };

} // namespace render
