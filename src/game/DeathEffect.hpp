#pragma once

#include "game/Effect.hpp"

namespace game
{

    class DeathEffect : public Effect
    {
    public:
        DeathEffect(std::vector<ColorCell> cells, unsigned int duration);
        void run() override;
    };

} // namespace game
