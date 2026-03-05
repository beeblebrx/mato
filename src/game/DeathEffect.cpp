#include "game/DeathEffect.hpp"

namespace game
{

    DeathEffect::DeathEffect(std::vector<ColorCell> cells, unsigned int duration)
        : Effect(std::move(cells), duration)
    {
        isPriority_ = true;
        pauses_ = true;
        isFinal_ = true;
    }

    void DeathEffect::run()
    {
        sf::Uint8 red = static_cast<sf::Uint8>(255 - (235 * tick_ / duration_));
        for (auto &cell : cells_)
            cell.color = sf::Color(red, 0, 0);
    }

} // namespace game
