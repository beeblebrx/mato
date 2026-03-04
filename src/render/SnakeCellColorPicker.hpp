#pragma once

#include "render/CellColorPicker.hpp"

namespace render
{

    class SnakeCellColorPicker : public CellColorPicker
    {
    public:
        sf::Color pick(const game::ColorCell &cell, game::Phase phase) const override;
    };

} // namespace render
