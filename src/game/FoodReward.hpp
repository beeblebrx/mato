#pragma once
#include <cstdint>

namespace game
{

    class FoodReward
    {
    public:
        void update();
        void reset();
        unsigned int value() const;

    private:
        static constexpr unsigned int kMax = 1000;
        static constexpr unsigned int kMin = 10;
        static constexpr unsigned int kDecayTicks = 200;
        unsigned int ticks_{};
    };

} // namespace game
