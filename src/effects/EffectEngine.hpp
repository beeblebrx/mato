#pragma once

#include <memory>
#include <vector>

#include "game/Types.hpp"
#include "effects/Effect.hpp"

namespace audio
{
    class SoundPlayer;
}

namespace effects
{

    class EffectEngine
    {
    public:
        void trigger(game::Event event, const std::vector<game::ColorCell> &snake);
        void run(const std::vector<game::ColorCell> &currentCells);
        void clear();

        void setSoundPlayer(audio::SoundPlayer *player);

        bool hasPausingEffect() const;
        bool hasEffects() const;
        bool hasActiveEffects() const;
        const std::vector<game::ColorCell> &cells() const;

    private:
        void add(std::unique_ptr<Effect> effect);

        std::vector<std::unique_ptr<Effect>> effects_;
        std::vector<game::ColorCell> outputCells_;
        audio::SoundPlayer *soundPlayer_ = nullptr;
    };

} // namespace effects
