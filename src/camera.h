#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera
{
    public:

    // Properties
    const float MAX_FOV = 100.f;
    const float MIN_FOV = 0.f;
    float mouseSensitivity = .1f;
    float yaw = -90.f;
    float pitch = 0.f;
    float fov = 45.f;
    bool isSprinting = false;
    glm::vec3 position;
    
    // Constructors
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up);
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, glm::vec3 worldUp);
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float yaw, float pitch, float movementSpeed, float mouseSensitivity);
    
    // Callback functions
    void processKeyboard(Camera_Movement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, GLboolean constraintPitch = true);
    void processMouseScroll(float yoffset);
    
    // Methods
    void lookAtPosition(glm::vec3 position);
    glm::mat4 getViewMatrix() const;
    glm::vec3 getFront() const;
    float getSpeed() const;

    private:

    // Camera vectors
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Properties
    float movementSpeed = 2.5f;
    float maxSpeed = 6.f;
    
    // Reconstruct vectors
    void updateCameraVectors();
};
#endif