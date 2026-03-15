#pragma once

#include "effects/sfx/SoundEffect.hpp"

namespace effects
{

    class DeathSoundEffect : public SoundEffect
    {
    public:
        explicit DeathSoundEffect(audio::SoundPlayer &player)
            : SoundEffect(player, game::Event::Death) {}
    };

} // namespace effects
