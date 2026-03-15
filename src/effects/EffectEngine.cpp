#include "effects/EffectEngine.hpp"

#include <algorithm>

#include "effects/CompositeEffect.hpp"
#include "effects/gfx/CelebrationEffect.hpp"
#include "effects/gfx/DeathEffect.hpp"
#include "effects/gfx/FoodEatEffect.hpp"
#include "effects/sfx/DeathSoundEffect.hpp"
#include "effects/sfx/FoodEatSoundEffect.hpp"
#include "effects/sfx/LevelCompleteSoundEffect.hpp"

namespace effects
{

    void EffectEngine::trigger(game::Event event, const std::vector<game::ColorCell> &snake)
    {
        switch (event)
        {
        case game::Event::FoodEaten:
        {
            auto gfx = std::make_unique<FoodEatEffect>(snake, 3);
            std::unique_ptr<Effect> sfx = soundPlayer_
                                              ? std::make_unique<FoodEatSoundEffect>(*soundPlayer_)
                                              : nullptr;
            add(std::make_unique<CompositeEffect>(std::move(gfx), std::move(sfx)));
            break;
        }
        case game::Event::Death:
        {
            auto gfx = std::make_unique<DeathEffect>(snake, 20);
            std::unique_ptr<Effect> sfx = soundPlayer_
                                              ? std::make_unique<DeathSoundEffect>(*soundPlayer_)
                                              : nullptr;
            add(std::make_unique<CompositeEffect>(std::move(gfx), std::move(sfx)));
            break;
        }
        case game::Event::LevelComplete:
        {
            auto gfx = std::make_unique<CelebrationEffect>(snake, 25);
            std::unique_ptr<Effect> sfx = soundPlayer_
                                              ? std::make_unique<LevelCompleteSoundEffect>(*soundPlayer_)
                                              : nullptr;
            add(std::make_unique<CompositeEffect>(std::move(gfx), std::move(sfx)));
            break;
        }
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

            effect->advance();
        }

        std::erase_if(effects_, [](const auto &e)
                      { return e->expired() && !e->isFinal(); });
    }

    void EffectEngine::clear()
    {
        effects_.clear();
        outputCells_.clear();
    }

    void EffectEngine::setSoundPlayer(audio::SoundPlayer *player)
    {
        soundPlayer_ = player;
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

} // namespace effects
