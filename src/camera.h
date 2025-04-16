#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// TODO: Add "focus" control: pressing 'f' to focus (once) at object

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    const float MAX_FOV = 100.f;

    float mouseSensitivity = .1f;
    float movementSpeed = 2.5f;
    float yaw = -90.f;
    float pitch = 0.f;
    float fov = 45.f;

    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up);
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, glm::vec3 worldUp);
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float yaw, float pitch, float movementSpeed, float mouseSensitivity);

    void processKeyboard(Camera_Movement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, GLboolean constraintPitch = true);
    void processMouseScroll(float yoffset);

    glm::mat4 getViewMatrix();

private:
    void updateCameraVectors();
};
#endif