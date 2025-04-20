#pragma once
#include "Luma/Core/Base.h"

namespace Luma
{
    namespace Core
    {
        struct TimeState
        {
            float currentTime = 0.f;
            float lastTime = 0.f;
            float deltaTime = 0.f;
            u32 targetFrameRate = 0;
        };

        void TimeStateInitialize(u32 targetFPS);
        void TimeStateUpdate();

        extern TimeState* Time;
    }
}
