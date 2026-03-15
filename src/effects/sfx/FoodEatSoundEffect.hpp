#pragma once

#include "effects/sfx/SoundEffect.hpp"

namespace effects
{

    class FoodEatSoundEffect : public SoundEffect
    {
    public:
        explicit FoodEatSoundEffect(audio::SoundPlayer &player)
            : SoundEffect(player, game::Event::FoodEaten) {}
    };

} // namespace effects
