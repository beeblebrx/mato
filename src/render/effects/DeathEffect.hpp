#pragma once

#include "render/effects/Effect.hpp"

namespace render
{

    class DeathEffect : public Effect
    {
    public:
        DeathEffect(std::vector<game::ColorCell> cells, unsigned int duration);
        void run() override;
    };

} // namespace render
