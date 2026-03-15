#pragma once

#include "audio/SoundPlayer.hpp"
#include "effects/Effect.hpp"
#include "game/Types.hpp"

namespace effects
{

    class SoundEffect : public Effect
    {
    public:
        SoundEffect(audio::SoundPlayer &player, game::Event event);
        void run() override;

    private:
        audio::SoundPlayer &player_;
        game::Event event_;
        bool played_ = false;
    };

} // namespace effects
