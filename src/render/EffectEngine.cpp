#include "render/EffectEngine.hpp"

#include <algorithm>

#include "render/effects/CelebrationEffect.hpp"
#include "render/effects/DeathEffect.hpp"
#include "render/effects/FoodEatEffect.hpp"

namespace render
{

    void EffectEngine::trigger(game::Event event, const std::vector<game::ColorCell> &snake)
    {
        switch (event)
        {
        case game::Event::FoodEaten:
            add(std::make_unique<FoodEatEffect>(snake, 3));
            break;
        case game::Event::Death:
            add(std::make_unique<DeathEffect>(snake, 20));
            break;
        case game::Event::LevelComplete:
            add(std::make_unique<CelebrationEffect>(snake, 25));
            break;
        case game::Event::ClearEffects:
            clear();
            break;
        }
    }

    void EffectEngine::add(std::unique_ptr<Effect> effect)
    {
        effects_.push_back(std::move(effect));
    }

    void EffectEngine::run(const std::vector<game::ColorCell> &currentCells)
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

    const std::vector<game::ColorCell> &EffectEngine::cells() const
    {
        return outputCells_;
    }

} // namespace render
