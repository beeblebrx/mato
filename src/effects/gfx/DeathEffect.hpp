#pragma once

#include "effects/Effect.hpp"

namespace effects
{

    class DeathEffect : public Effect
    {
    public:
        DeathEffect(std::vector<game::ColorCell> cells, unsigned int duration);
        void run() override;
    };

} // namespace effects
