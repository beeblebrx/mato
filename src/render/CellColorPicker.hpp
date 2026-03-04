#pragma once

#include <SFML/Graphics/Color.hpp>

#include "game/GameState.hpp"
#include "game/Types.hpp"

namespace render
{

    class CellColorPicker
    {
    public:
        virtual ~CellColorPicker() = default;
        virtual sf::Color pick(const game::ColorCell &cell, game::Phase phase) const
        {
            return cell.color;
        }
    };

} // namespace render
