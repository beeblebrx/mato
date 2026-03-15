#pragma once

#include <memory>

#include "effects/Effect.hpp"

namespace effects
{

    class CompositeEffect : public Effect
    {
    public:
        CompositeEffect(std::unique_ptr<Effect> gfx, std::unique_ptr<Effect> sfx);
        void run() override;
        void advance() override;

    private:
        std::unique_ptr<Effect> gfx_;
        std::unique_ptr<Effect> sfx_;
    };

} // namespace effects
