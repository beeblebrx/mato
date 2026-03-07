#include "game/FoodReward.hpp"
#include <algorithm>

namespace game {

void FoodReward::update() {
    if (ticks_ < kDecayTicks) ++ticks_;
}

void FoodReward::reset() {
    ticks_ = 0;
}

unsigned int FoodReward::value() const {
    unsigned int decay = (kMax - kMin) * ticks_ / kDecayTicks;
    return kMax - decay;
}

} // namespace game
