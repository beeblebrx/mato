#pragma once

#include <memory>
#include <vector>

#include "game/Effect.hpp"

namespace game
{

    class EffectEngine
    {
    public:
        void add(std::unique_ptr<Effect> effect);
        void run(const std::vector<ColorCell> &currentCells);
        void clear();

        bool hasPausingEffect() const;
        bool hasEffects() const;
        bool hasActiveEffects() const;
        const std::vector<ColorCell> &cells() const;

    private:
        std::vector<std::unique_ptr<Effect>> effects_;
        std::vector<ColorCell> outputCells_;
    };

} // namespace game
