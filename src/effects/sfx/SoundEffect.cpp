#include "effects/sfx/SoundEffect.hpp"

namespace effects
{

    SoundEffect::SoundEffect(audio::SoundPlayer &player, game::Event event)
        : Effect({}, 1), player_(player), event_(event)
    {
    }

    void SoundEffect::run()
    {
        if (!played_)
        {
            player_.play(event_);
            played_ = true;
        }
    }

} // namespace effects
