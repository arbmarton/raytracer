#include "Camera.h"

#include "gtc/matrix_transform.hpp"


glm::mat4 Camera::getLookAt() const
{
    return glm::lookAt(position, position + front, up);
}

void Camera::processMovement(const float xOffset, const float yOffset)
{
    yaw += xOffset * sensitivity;
    pitch += yOffset * sensitivity;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }

    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    glm::vec3 frontTemp;
    frontTemp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    frontTemp.y = sin(glm::radians(pitch));
    frontTemp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(frontTemp);
}