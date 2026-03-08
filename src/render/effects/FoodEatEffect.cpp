#include "render/effects/FoodEatEffect.hpp"

namespace render
{

    FoodEatEffect::FoodEatEffect(std::vector<game::ColorCell> cells, unsigned int duration)
        : Effect(std::move(cells), duration)
    {
        originalColors_.reserve(cells_.size());
        for (const auto &cell : cells_)
            originalColors_.push_back(cell.color);
    }

    void FoodEatEffect::run()
    {
        float blend = (duration_ - tick_) / static_cast<float>(duration_);
        for (std::size_t i = 0; i < cells_.size(); ++i)
        {
            const sf::Color &orig = originalColors_[i];
            const sf::Color &flash = game::kSnakeEatFlashColor;
            cells_[i].color = sf::Color(
                static_cast<sf::Uint8>(orig.r + blend * (flash.r - orig.r)),
                static_cast<sf::Uint8>(orig.g + blend * (flash.g - orig.g)),
                static_cast<sf::Uint8>(orig.b + blend * (flash.b - orig.b)));
        }
    }

} // namespace render
