#pragma once

namespace Luma
{
    namespace UI
    {
        void SetupContext();
        void DestroyContext();
        void HandleEvents(void* event);
        void BeginFrome();
        void EndFrame();
        void Display();
    }
}
