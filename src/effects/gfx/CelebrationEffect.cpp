#include "effects/gfx/CelebrationEffect.hpp"

#include <cmath>

namespace effects
{

    namespace
    {
        constexpr float kHueShiftPerTick = 18.0f;
        constexpr float kRainbowCycles = 1.5f;

        sf::Color hsvToRgb(float h, float s, float v)
        {
            float c = v * s;
            float x = c * (1.0f - std::abs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
            float m = v - c;
            float r, g, b;
            if (h < 60)
            {
                r = c;
                g = x;
                b = 0;
            }
            else if (h < 120)
            {
                r = x;
                g = c;
                b = 0;
            }
            else if (h < 180)
            {
                r = 0;
                g = c;
                b = x;
            }
            else if (h < 240)
            {
                r = 0;
                g = x;
                b = c;
            }
            else if (h < 300)
            {
                r = x;
                g = 0;
                b = c;
            }
            else
            {
                r = c;
                g = 0;
                b = x;
            }
            return sf::Color{
                static_cast<sf::Uint8>((r + m) * 255),
                static_cast<sf::Uint8>((g + m) * 255),
                static_cast<sf::Uint8>((b + m) * 255),
            };
        }
    }

    CelebrationEffect::CelebrationEffect(std::vector<game::ColorCell> cells, unsigned int duration)
        : Effect(std::move(cells), duration)
    {
        pauses_ = true;
        isFinal_ = true;
    }

    void CelebrationEffect::run()
    {
        float n = static_cast<float>(cells_.size());
        float offset = -static_cast<float>(tick_) * kHueShiftPerTick;

        for (std::size_t i = 0; i < cells_.size(); ++i)
        {
            float hue = std::fmod(kRainbowCycles * 360.0f * static_cast<float>(i) / n + offset, 360.0f);
            if (hue < 0.0f)
                hue += 360.0f;
            cells_[i].color = hsvToRgb(hue, 1.0f, 1.0f);
        }
    }

} // namespace effects
