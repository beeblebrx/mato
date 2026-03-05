#include "game/CelebrationEffect.hpp"

#include <array>

namespace game
{

    namespace
    {
        const std::array<sf::Color, 6> kCelebrationColors = {
            sf::Color{240, 240, 240},
            sf::Color{33, 150, 83},
            sf::Color{255, 230, 0},
            sf::Color{139, 90, 43},
            sf::Color{139, 0, 0},
            sf::Color{255, 80, 0},
        };

        constexpr unsigned int kColorRegenInterval = 2;
    }

    CelebrationEffect::CelebrationEffect(std::vector<ColorCell> cells, unsigned int duration, std::mt19937 &rng)
        : Effect(std::move(cells), duration), rng_(rng)
    {
        pauses_ = true;
        isFinal_ = true;
    }

    void CelebrationEffect::run()
    {
        if (tick_ == 0 || tick_ - lastRegenTick_ >= kColorRegenInterval)
        {
            cachedColors_.resize(cells_.size());
            std::uniform_int_distribution<std::size_t> dist(0, kCelebrationColors.size() - 1);
            for (auto &color : cachedColors_)
                color = kCelebrationColors[dist(rng_)];
            lastRegenTick_ = tick_;
        }

        for (std::size_t i = 0; i < cells_.size(); ++i)
            cells_[i].color = cachedColors_[i];
    }

} // namespace game
