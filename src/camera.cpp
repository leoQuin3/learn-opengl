#include <iostream>
#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up) : position(position), front(front), up(up), worldUp(up)
{
    updateCameraVectors();
}

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, glm::vec3 worldUp) : position(position), front(front), up(up), worldUp(worldUp)
{
    updateCameraVectors();
}

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float yaw, float pitch, float movementSpeed, float mouseSensitivity) : position(position), front(front), up(up), yaw(yaw), pitch(pitch), movementSpeed(movementSpeed), mouseSensitivity(mouseSensitivity)
{
    updateCameraVectors();
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;

    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
    if (direction == UP)
        position += up * velocity;
    if (direction == DOWN)
        position -= up * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constraintPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constraintPitch) {
        if (pitch > 89.f)
            pitch = 89.f;
        if (pitch < -89.f)
            pitch = -89.f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
    fov -= (float)yoffset;
    if (fov < MIN_FOV)
        fov = MIN_FOV;
    if (fov > MAX_FOV)
        fov = MAX_FOV;
}

void Camera::lookAtPosition(glm::vec3 position)
{
    glm::vec3 lookDir = glm::normalize(position - this->position);
    this->pitch = glm::degrees(glm::asin(lookDir.y));
    this->yaw = glm::degrees(glm::atan(lookDir.z, lookDir.x));
    
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    glm::mat4 translation = glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -position.x, -position.y, -position.z, 1
    );
    glm::mat4 rotation = glm::mat4(
        right.x, up.x, -front.x, 0,
        right.y, up.y, -front.y, 0,
        right.z, up.z, -front.z, 0,
        0, 0, 0, 1
    );
    return rotation * translation;
}

void Camera::updateCameraVectors()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}