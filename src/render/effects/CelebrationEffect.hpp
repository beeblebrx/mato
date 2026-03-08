#pragma once

#include "render/effects/Effect.hpp"

namespace render
{

    class CelebrationEffect : public Effect
    {
    public:
        CelebrationEffect(std::vector<game::ColorCell> cells, unsigned int duration);
        void run() override;
    };

} // namespace render
