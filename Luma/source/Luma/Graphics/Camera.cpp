#include "Luma/Graphics/Camera.h"

#include "Luma/Core/Application.h"
#include "Luma/Core/Input.h"
#include "Luma/Core/Log.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Luma
{
    namespace Graphics
    {
        void UpdateCameraArcBall(Camera& camera);

        glm::vec3 Camera::GetViewDirection()
        {
            return glm::normalize(target - position);
        }

        glm::vec3 Camera::GetRightVector()
        {
            return glm::normalize(glm::cross(GetViewDirection(), up));
        }

        void UpdateCamera(Camera& camera, CameraType type)
        {
            switch (type)
            {
                case CAMERA_TYPE_ARCBALL:
                    UpdateCameraArcBall(camera);
                    break;

                default:
                    break;
            }
        }

        void UpdateCameraArcBall(Camera& camera)
        {
            Core::ApplicationConfig& appInfo = Core::GetApplicationInfo();

            if (Core::IsKeyDown(Core::KEY_S))
                camera.position -= camera.moveSpeed * camera.GetViewDirection();

            if (Core::IsKeyDown(Core::KEY_W))
                camera.position += camera.moveSpeed * camera.GetViewDirection();

            if (Core::IsKeyDown(Core::KEY_A))
                camera.position -= camera.moveSpeed * camera.GetRightVector();

            if (Core::IsKeyDown(Core::KEY_D))
                camera.position += camera.moveSpeed * camera.GetRightVector();

            if (Core::IsMouseDown(Core::MOUSE_BUTTON_RIGHT))
            {
                glm::vec4 position = glm::vec4(V3_OPEN(camera.position), 1.f);
                glm::vec4 pivot = glm::vec4(V3_OPEN(camera.target), 1.f);

                glm::vec2 deltaAngle;
                deltaAngle.x = 2.f * M_PI / appInfo.windowWidth;
                deltaAngle.y = M_PI / appInfo.windowHeight;

                glm::vec2 angle;
                angle.x = Core::GetMouseRelative().x * deltaAngle.x * camera.lookSensitivity;
                angle.y = Core::GetMouseRelative().y * deltaAngle.y * camera.lookSensitivity;

                float currentPitch = glm::asin(glm::dot(camera.GetViewDirection(), camera.up)); // Angle from horizontal
                float newPitch = currentPitch + angle.y;

                const float maxPitch = glm::radians(85.0f);
                if (newPitch > maxPitch)
                    angle.y = maxPitch - currentPitch;
                else if (newPitch < -maxPitch)
                    angle.y = -maxPitch - currentPitch;

                // Rotation around up and right vectors
                glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.f), angle.x, camera.up);
                glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.f), angle.y, camera.GetRightVector());

                glm::mat4 rotationMatrix = rotationMatrixX * rotationMatrixY;
                glm::vec4 newPosition = rotationMatrix * (position - pivot) + pivot;

                camera.position = glm::vec3(newPosition);
            }

            camera.view = glm::lookAt(camera.position, camera.target, camera.up);
        }
    }
}
