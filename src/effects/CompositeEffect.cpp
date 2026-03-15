#include "effects/CompositeEffect.hpp"

namespace effects
{

    CompositeEffect::CompositeEffect(std::unique_ptr<Effect> gfx, std::unique_ptr<Effect> sfx)
        : Effect(gfx->cells(), gfx->duration()), gfx_(std::move(gfx)), sfx_(std::move(sfx))
    {
        isPriority_ = gfx_->isPriority();
        pauses_ = gfx_->pauses();
        isFinal_ = gfx_->isFinal();
    }

    void CompositeEffect::run()
    {
        gfx_->run();
        cells_ = gfx_->cells();

        if (sfx_ && !sfx_->expired())
            sfx_->run();
    }

    void CompositeEffect::advance()
    {
        ++tick_;
        gfx_->advance();
        if (sfx_)
            sfx_->advance();
    }

} // namespace effects
