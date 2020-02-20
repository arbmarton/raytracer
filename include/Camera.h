#pragma once

#include "glm.hpp"

class Camera
{
public:
    static Camera& intance()
    {
        static Camera* inst = nullptr;
        if (!inst)
        {
            inst = new Camera();
        }
        return *inst;
    }

    void setPosition(const glm::vec3 pos)
    {
        position = pos;
    }
    glm::vec3 getPosition() const
    {
        return position;
    }

    void setSpeed(const float val)
    {
        speed = val;
    }
    float getSpeed() const
    {
        return speed;
    }

    void setPitch(const float val)
    {
        pitch = val;
    }
    float getPitch() const
    {
        return pitch;
    }

    void setYaw(const float val)
    {
        yaw = val;
    }
    float getYaw() const
    {
        return yaw;
    }

    void setFov(const float val)
    {
        fov = val;
    }
    float getFov() const
    {
        return fov;
    }

    void moveForward()
    {
        position += speed * front;
    }

    void moveBackward()
    {
        position -= speed * front;
    }

    void moveLeft()
    {
        position -= glm::normalize(glm::cross(front, up)) * speed;
    }

    void moveRight()
    {
        position += glm::normalize(glm::cross(front, up)) * speed;
    }

    glm::vec3 getDirection() const
    {
        return front;
    }

    glm::vec3 getUp() const
    {
        return up;
    }

    glm::mat4 getLookAt() const;
    void processMovement(const float xOffset, const float yOffset);

private:
    Camera() = default;

    glm::vec3 position{ 0, 0, 0 };
    glm::vec3 front{ 0, 0, -1 };
    glm::vec3 up{ 0, 1, 0 };

    float pitch{ 0 };
    float yaw{ -90 };

    float fov{ 45.0f };

    float speed{ 0.05f };

    float sensitivity{ 0.05f };
};