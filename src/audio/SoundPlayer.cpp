#include "audio/SoundPlayer.hpp"

namespace audio
{

    SoundPlayer::SoundPlayer(const std::string &assetDir)
        : assetDir_(assetDir)
    {
        loadSound(game::Event::FoodEaten, "eat.mp3");
        loadSound(game::Event::Death, "death.mp3");
        loadSound(game::Event::LevelComplete, "level_complete.mp3");
    }

    void SoundPlayer::loadSound(game::Event event, const std::string &filename)
    {
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(assetDir_ + filename))
            buffers_.emplace(event, std::move(buffer));
    }

    void SoundPlayer::play(game::Event event)
    {
        auto it = buffers_.find(event);
        if (it == buffers_.end())
            return;

        for (auto &sound : sounds_)
        {
            if (sound.getStatus() == sf::Sound::Stopped)
            {
                sound.setBuffer(it->second);
                sound.play();
                return;
            }
        }
    }

} // namespace audio
