#pragma once
#include "Luma/Core/Base.h"
#include <glm/glm.hpp>

namespace Luma
{
    namespace Graphics
    {
        enum CameraType : u32
        {
            CAMERA_TYPE_ARCBALL = 0
        };

        struct Camera
        {
            float moveSpeed = 0.f;
            float lookSensitivity = 0.f;
            float fov = 45.f;
            glm::vec3 position = glm::vec3(0.f);
            glm::vec3 target = glm::vec3(0.f);
            glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
            glm::mat4 view = glm::mat4(1.f);

            glm::vec3 GetViewDirection();
            glm::vec3 GetRightVector();
        };

        void UpdateCamera(Camera& camera, CameraType type);
    }
}
