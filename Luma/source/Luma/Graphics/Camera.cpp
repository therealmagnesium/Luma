#include "Luma/Graphics/Camera.h"

#include "Luma/Core/Application.h"
#include "Luma/Core/Input.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Luma
{
    namespace Graphics
    {
        void UpdateCameraArcBall(Camera& camera);

        glm::vec3 Camera::GetViewDirection()
        {
            return glm::normalize(-glm::transpose(view)[2]);
        }

        glm::vec3 Camera::GetRightVector()
        {
            return glm::normalize(glm::transpose(view)[0]);
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

            if (Core::IsMouseDown(Core::MOUSE_BUTTON_LEFT))
            {
                glm::vec4 position = glm::vec4(V3_OPEN(camera.position), 1.f);
                glm::vec4 pivot = glm::vec4(V3_OPEN(camera.target), 1.f);

                glm::vec2 deltaAngle;
                deltaAngle.x = 2.f * M_PI / appInfo.windowWidth;
                deltaAngle.y = M_PI / appInfo.windowHeight;

                glm::vec2 angle;
                angle.x = Core::GetMouseRelative().x * deltaAngle.x * camera.lookSensitivity;
                angle.y = Core::GetMouseRelative().y * deltaAngle.y * camera.lookSensitivity;

                float cosAngle = glm::dot(camera.GetViewDirection(), camera.up);
                if (cosAngle * glm::sign(deltaAngle.y) > 0.99f)
                    deltaAngle.y = 0.f;

                glm::mat4 rotationMatrixX = glm::mat4(1.f);
                rotationMatrixX = glm::rotate(rotationMatrixX, angle.x, camera.up);
                position = (rotationMatrixX * (position - pivot)) + pivot;

                glm::mat4 rotationMatrixY = glm::mat4(1.f);
                rotationMatrixY = glm::rotate(rotationMatrixY, angle.y, camera.GetRightVector());
                glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;

                camera.position = finalPosition;
            }

            camera.view = glm::lookAt(camera.position, camera.target, camera.up);
        }
    }
}
