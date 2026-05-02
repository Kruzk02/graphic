#pragma once

#include <cstdint>

#include "glad/glad.h"
#include <glm/gtc/matrix_transform.hpp>

namespace CameraSettingsDefault
{
constexpr float Yaw = -90.0f;
constexpr float Pitch = 0.0f;
constexpr float Speed = 2.5f;
constexpr float Sensitivity = 0.1f;
constexpr float Zoom = 45.0f;
}; // namespace CameraSettingsDefault

namespace CameraLimit
{
constexpr float HighPitch = 89.0f;
constexpr float LowPich = -89.0f;
constexpr float MaxZoom = 45.0f;
} // namespace CameraLimit

enum class CameraMovement : std::uint8_t
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

struct CameraSettings
{
    float yaw = CameraSettingsDefault::Yaw;
    float pitch = CameraSettingsDefault::Pitch;
    float speed = CameraSettingsDefault::Speed;
    float sensitivity = CameraSettingsDefault::Sensitivity;
    float zoom = CameraSettingsDefault::Zoom;
};

struct CameraPosition
{
    float x;
    float y;
    float z;

    CameraPosition(float posX, float posY, float posZ) : x(posX), y(posY), z(posZ) {}
};

struct CameraUp
{
    float x;
    float y;
    float z;

    CameraUp(float upX, float upY, float upZ) : x(upX), y(upY), z(upZ) {}
};

struct CameraAttitude
{
    float yaw;
    float pitch;
};

struct CameraParam
{
    CameraPosition position;
    CameraUp up;
    CameraAttitude attitude;

    CameraParam(CameraPosition pos, CameraUp up, CameraAttitude att)
        : position(pos.x, pos.y, pos.z), up(up.x, up.y, up.z), attitude(att.yaw, att.pitch)
    {
    }
};

class Camera
{
  public:
    explicit Camera(const CameraPosition &pos, const CameraUp &up,
                    const CameraSettings &settings = {})
        : Camera(CameraParam(pos, up, CameraAttitude(settings.yaw, settings.pitch)), settings)
    {
        updateCameraVectors();
    }

    Camera(const CameraParam &cameraParam, const CameraSettings &settings = {})
        : cameraParam(cameraParam), front(glm::vec3(0.0f, 0.0f, -1.0f)),
          movementSpeed(settings.speed), mouseSensitivity(settings.sensitivity), zoom(settings.zoom)
    {
        this->position =
            glm::vec3(cameraParam.position.x, cameraParam.position.y, cameraParam.position.z);
        this->worldUp = glm::vec3(cameraParam.up.x, cameraParam.up.y, cameraParam.up.z);
        this->cameraParam.attitude.yaw = cameraParam.attitude.yaw;
        this->cameraParam.attitude.pitch = cameraParam.attitude.pitch;
        updateCameraVectors();
    }

    [[nodiscard]] auto getViewMatrix() const -> glm::mat4
    {
        return glm::lookAt(position, position + front, up);
    }

    void processKeyboard(const CameraMovement direction, const float deltaTime)
    {
        const float velocity = movementSpeed * deltaTime;
        if (direction == CameraMovement::FORWARD)
            position += front * velocity;
        if (direction == CameraMovement::BACKWARD)
            position -= front * velocity;
        if (direction == CameraMovement::LEFT)
            position -= right * velocity;
        if (direction == CameraMovement::RIGHT)
            position += right * velocity;
    }

    void processMouseMovement(float xOffset, float yOffset, const GLboolean constrainPitch = true)
    {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        cameraParam.attitude.yaw += xOffset;
        cameraParam.attitude.pitch += yOffset;

        if (constrainPitch)
        {
            if (cameraParam.attitude.pitch > CameraLimit::HighPitch)
                cameraParam.attitude.pitch = CameraLimit::HighPitch;
            if (cameraParam.attitude.pitch < CameraLimit::LowPich)
                cameraParam.attitude.pitch = CameraLimit::LowPich;
        }

        updateCameraVectors();
    }

    void processMouseScroll(const float yOffset)
    {
        zoom -= yOffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > CameraLimit::MaxZoom)
            zoom = CameraLimit::MaxZoom;
    }

    [[nodiscard]] auto getPosition() const -> const glm::vec3 &
    {
        return position;
    }
    [[nodiscard]] auto getFront() const -> const glm::vec3 &
    {
        return front;
    }

    [[nodiscard]] auto getZoom() const -> float
    {
        return zoom;
    }

  private:
    glm::vec3 position{};
    glm::vec3 front;
    glm::vec3 up{};
    glm::vec3 right{};
    glm::vec3 worldUp{};

    CameraParam cameraParam;
    float movementSpeed;
    float mouseSensitivity{};
    float zoom;

    void updateCameraVectors()
    {
        glm::vec3 v_front;
        v_front.x = glm::cos(glm::radians(cameraParam.attitude.yaw)) *
                    glm::cos(glm::radians(cameraParam.attitude.pitch));
        v_front.y = glm::sin(glm::radians(cameraParam.attitude.pitch));
        v_front.z = glm::sin(glm::radians(cameraParam.attitude.yaw)) *
                    glm::cos(glm::radians(cameraParam.attitude.pitch));
        front = glm::normalize(v_front);

        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};
