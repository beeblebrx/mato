#pragma once

#include "effects/sfx/SoundEffect.hpp"

namespace effects
{

    class LevelCompleteSoundEffect : public SoundEffect
    {
    public:
        explicit LevelCompleteSoundEffect(audio::SoundPlayer &player)
            : SoundEffect(player, game::Event::LevelComplete) {}
    };

} // namespace effects
