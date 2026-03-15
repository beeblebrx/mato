#pragma once

#include <array>
#include <map>
#include <string>

#include <SFML/Audio.hpp>

#include "game/Types.hpp"

namespace audio
{

    class SoundPlayer
    {
    public:
        explicit SoundPlayer(const std::string &assetDir);

        void play(game::Event event);

    private:
        void loadSound(game::Event event, const std::string &filename);

        std::string assetDir_;
        std::map<game::Event, sf::SoundBuffer> buffers_;
        static constexpr std::size_t kPoolSize = 8;
        std::array<sf::Sound, kPoolSize> sounds_;
    };

} // namespace audio
