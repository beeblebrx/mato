#pragma once

#include "game/Effect.hpp"

namespace game
{

    class CelebrationEffect : public Effect
    {
    public:
        CelebrationEffect(std::vector<ColorCell> cells, unsigned int duration);
        void run() override;
    };

} // namespace game
