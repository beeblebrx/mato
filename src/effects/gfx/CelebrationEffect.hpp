#pragma once

#include "effects/Effect.hpp"

namespace effects
{

    class CelebrationEffect : public Effect
    {
    public:
        CelebrationEffect(std::vector<game::ColorCell> cells, unsigned int duration);
        void run() override;
    };

} // namespace effects
