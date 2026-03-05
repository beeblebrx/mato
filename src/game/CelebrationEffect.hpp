#pragma once

#include <random>

#include "game/Effect.hpp"

namespace game
{

    class CelebrationEffect : public Effect
    {
    public:
        CelebrationEffect(std::vector<ColorCell> cells, unsigned int duration, std::mt19937 &rng);
        void run() override;

    private:
        std::mt19937 &rng_;
        std::vector<sf::Color> cachedColors_;
        unsigned int lastRegenTick_ = 0;
    };

} // namespace game
