#include "game/EffectEngine.hpp"

#include <algorithm>

namespace game
{

    void EffectEngine::add(std::unique_ptr<Effect> effect)
    {
        effects_.push_back(std::move(effect));
    }

    void EffectEngine::run(const std::vector<ColorCell> &currentCells)
    {
        bool hasFinalExpired = std::any_of(effects_.begin(), effects_.end(),
                                             [](const auto &e)
                                             { return e->isFinal() && e->expired(); });

        if (!hasFinalExpired)
            outputCells_ = currentCells;

        if (effects_.empty())
            return;

        bool hasPriority = std::any_of(effects_.begin(), effects_.end(),
                                       [](const auto &e)
                                       { return e->isPriority(); });

        for (auto &effect : effects_)
        {
            if (effect->expired())
                continue;
            if (hasPriority && !effect->isPriority())
                continue;

            effect->run();

            const auto &effectCells = effect->cells();
            std::size_t count = std::min(effectCells.size(), outputCells_.size());
            for (std::size_t i = 0; i < count; ++i)
                outputCells_[i].color = effectCells[i].color;

            ++effect->tick_;
        }

        std::erase_if(effects_, [](const auto &e)
                      { return e->expired() && !e->isFinal(); });
    }

    void EffectEngine::clear()
    {
        effects_.clear();
        outputCells_.clear();
    }

    bool EffectEngine::hasPausingEffect() const
    {
        return std::any_of(effects_.begin(), effects_.end(),
                           [](const auto &e)
                           { return e->pauses() && !e->expired(); });
    }

    bool EffectEngine::hasEffects() const
    {
        return !effects_.empty();
    }

    bool EffectEngine::hasActiveEffects() const
    {
        return std::any_of(effects_.begin(), effects_.end(),
                           [](const auto &e)
                           { return !e->expired(); });
    }

    const std::vector<ColorCell> &EffectEngine::cells() const
    {
        return outputCells_;
    }

} // namespace game
