#include "Luma/Core/Time.h"
#include "Luma/Core/Log.h"

#include <SDL3/SDL_timer.h>
#include <math.h>

namespace Luma
{
    namespace Core
    {
        static bool isInitialized = false;
        static TimeState state;
        TimeState* Time = NULL;

        void TimeStateInitialize(u32 targetFPS)
        {
            if (isInitialized)
            {
                WARN("%s", "Cannot initialize the time state more than once");
                return;
            }

            state.targetFrameRate = targetFPS;

            Time = &state;
            isInitialized = true;
            INFO("The time state was initialized successfully");
        }

        void TimeStateUpdate()
        {
            const float ticksPerFrame = 1000.f / state.targetFrameRate;

            state.currentTime = SDL_GetTicks() / 1000.f;
            state.deltaTime = state.currentTime - state.lastTime;
            state.lastTime = state.currentTime;

            if (state.deltaTime < ticksPerFrame)
                SDL_Delay(ticksPerFrame - state.deltaTime);
        }

    }
}
