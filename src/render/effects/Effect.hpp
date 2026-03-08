#pragma once

#include <vector>

#include "game/Types.hpp"

namespace render
{

    class Effect
    {
    public:
        Effect(std::vector<game::ColorCell> cells, unsigned int duration)
            : cells_(std::move(cells)), duration_(duration) {}
        virtual ~Effect() = default;

        virtual void run() = 0;

        bool isPriority() const { return isPriority_; }
        bool pauses() const { return pauses_; }
        bool isFinal() const { return isFinal_; }
        unsigned int duration() const { return duration_; }
        bool expired() const { return tick_ >= duration_; }
        const std::vector<game::ColorCell> &cells() const { return cells_; }

    protected:
        std::vector<game::ColorCell> cells_;
        unsigned int duration_;
        unsigned int tick_ = 0;
        bool isPriority_ = false;
        bool pauses_ = false;
        bool isFinal_ = false;

        friend class EffectEngine;
    };

} // namespace render
