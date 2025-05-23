#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.cpp"
#include "camera.h"
#include "stb_image.h"

const unsigned int SCREEN_WIDTH = 1080;
const unsigned int SCREEN_HEIGHT = 1080;

// NOTE: Make sure the executable is one directory down from project root (ie. something like '{root}/build/' minus the quotes)
const char* VERTEX_FILE_PATH = "../vert.glsl";
const char* CUBE_FRAG_FILE_PATH = "../cube_frag.glsl";
const char* LIGHT_FRAG_FILE_PATH = "../light_frag.glsl";
const char* DIFFUSE_TEXTURE_PATH = "../assets/container2.png";
const char* SPEC_TEXTURE_PATH = "../assets/container2_specular.png";

float deltaTime = 0.f;
float lastFrame = 0.f;
float totalTime = 0.f;

float lastX = SCREEN_WIDTH / 2.f;
float lastY = SCREEN_HEIGHT / 2.f;

// Camera
const glm::vec3 INITIAL_CAM_POS = glm::vec3(0.f, 0.f, 3.f);
bool firstMouse = true;
Camera camera = Camera(INITIAL_CAM_POS, glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));

// Flashlight
bool isFlashlightOn = false;
bool canToggleFlashlight = true;

void processInput(GLFWwindow *window);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void mouseCallback(GLFWwindow *window, double xPos, double yPos);
void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);
unsigned int loadTexture(char const *path);

/*
    TODO:
        - Move onto Model Loading!
*/

int main()
{
    // Initialize glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Register functions
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // Load functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Create Shader Programs
    Shader cubeShader(VERTEX_FILE_PATH, CUBE_FRAG_FILE_PATH);
    Shader lightShader(VERTEX_FILE_PATH, LIGHT_FRAG_FILE_PATH);

    // Verticies of a cube
    float vertices[] = {
        // Position          // Normal            // Texture Coordinates
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    // Cube positions
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // Point light positions
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    glm::vec3 pointLightColors[] = {
        glm::vec3(1.f, 1.f, .2f),
        glm::vec3(1.f, .15f, .0f),
        glm::vec3(1.f, 1.f, 1.f),
        glm::vec3(0.f, 0.2f, 1.f)
    };

    // -- Cube VAO --
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);   // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // Texture Coordinates
    glEnableVertexAttribArray(2);

    // -- Light VAO --
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);   // Position
    glEnableVertexAttribArray(0);

    // Create lighting maps
    unsigned int diffuseMap = loadTexture(DIFFUSE_TEXTURE_PATH);
    unsigned int specularMap = loadTexture(SPEC_TEXTURE_PATH);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        
        glClearColor(.8f, .55f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Activate cube shader
        cubeShader.use();
        
        // Cube lighting maps
        cubeShader.setInt("material.diffuse", 0);
        cubeShader.setInt("material.specular", 1);
        cubeShader.setInt("material.emission", 2);
        cubeShader.setFloat("material.shininess", 64.f);

        // Direcitonal lighting
        cubeShader.setVec3("dirLight.direction", glm::vec3(-.2f, -1.f, -.3f));
        cubeShader.setVec3("dirLight.ambient", glm::vec3(.0f));
        cubeShader.setVec3("dirLight.diffuse", glm::vec3(.4f));
        cubeShader.setVec3("dirLight.specular", glm::vec3(.5f));

        // Point lighting
        for (unsigned int i = 0; i < 4; i++)
        {
            std::stringstream uniformName;
            uniformName << "pointLights[" << i << "].";

            cubeShader.setVec3(uniformName.str() + "position", pointLightPositions[i]);
            cubeShader.setVec3(uniformName.str() + "ambient", pointLightColors[i] * 0.1f);
            cubeShader.setVec3(uniformName.str() + "diffuse", pointLightColors[i]);
            cubeShader.setVec3(uniformName.str() + "specular", pointLightColors[i]);
            cubeShader.setFloat(uniformName.str() + "constant", 1.f);
            cubeShader.setFloat(uniformName.str() + "linear", .09f);
            cubeShader.setFloat(uniformName.str() + "quadratic", .032f);
        }

        // Spotlight
        cubeShader.setVec3("spotLight.position", camera.position);
        cubeShader.setVec3("spotLight.direction", camera.getFront());
        cubeShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(8.5f)));
        cubeShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(11.5f)));
        cubeShader.setFloat("spotLight.constant", 1.f);
        cubeShader.setFloat("spotLight.linear", .09f);
        cubeShader.setFloat("spotLight.quadratic", .032f);
        cubeShader.setVec3("spotLight.ambient", glm::vec3(.05f) * static_cast<float>(isFlashlightOn));
        cubeShader.setVec3("spotLight.diffuse", glm::vec3(2.f) * static_cast<float>(isFlashlightOn));
        cubeShader.setVec3("spotLight.specular", glm::vec3(1.f) * static_cast<float>(isFlashlightOn));

        // Cube transformations
        glm::mat4 model = glm::mat4(1.f);
        glm::mat3 normalModel = glm::transpose(glm::inverse(model));
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 100.f);
        cubeShader.setMat4("model", model);
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", projection);
        cubeShader.setMat3("normalModel", normalModel);
        cubeShader.setVec3("viewPos", camera.position);
        
        // Cube textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // Render cubes
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            float angle = 20.f * i;
            model = glm::mat4(1.f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.f, .3f, .5f));
            normalModel = glm::transpose(glm::inverse(model));

            cubeShader.setMat4("model", model);
            cubeShader.setMat3("normalModel", normalModel);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Render point light cubes
        glBindVertexArray(lightVAO);
        size_t pointLightCount = sizeof(pointLightPositions) / sizeof(pointLightPositions[0]);
        for (unsigned int i = 0; i < pointLightCount; i++)
        {
            model = glm::mat4(1.f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(.5f));

            lightShader.use();
            lightShader.setMat4("model", model);
            lightShader.setMat4("view", view);
            lightShader.setMat4("projection", projection);
            lightShader.setVec3("lightColor", pointLightColors[i]);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

// Set viewport whenever the window resizes
void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Process movement and key input
void processInput(GLFWwindow *window)
{
    // Quit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    
    // Strafing
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::RIGHT, deltaTime);

    // Sprinting
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.isSprinting = true;
    else
        camera.isSprinting = false;

    // Vertical Movement
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::DOWN, deltaTime);
    
    // Focus on cube
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        camera.lookAtPosition(glm::vec3(0.f));

    // Reset position and rotation
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        camera.position = INITIAL_CAM_POS;
        camera.lookAtPosition(glm::vec3(0.f));
    }

    // Toggle flashlight
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && canToggleFlashlight)
    {
        canToggleFlashlight = false;
        isFlashlightOn = !isFlashlightOn ? true : false;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE)
        canToggleFlashlight = true;
}

// Rotate camera using mouse
void mouseCallback(GLFWwindow *window, double xPos, double yPos)
{
    // Avoid jerking within the first frame
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = static_cast<float>(xPos) - lastX;
    float yOffset = lastY - static_cast<float>(yPos);
    lastX = xPos;
    lastY = yPos;

    camera.processMouseMovement(xOffset, yOffset, GL_TRUE);
}

// Control zoom using mouse scroll
void scrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    camera.processMouseScroll(static_cast<float>(yOffset));
}

// Load texture
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        switch (nrComponents)
        {
            case 1:
                format = GL_RED;
                break;
            case 2:
                format = GL_RG;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
            default:
                format = GL_RGB;
                break;
        }
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "ERROR::Failed to load texture at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}