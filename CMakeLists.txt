cmake_minimum_required(VERSION 3.28.3)

project(learn_opengl_linux_project VERSION 0.1.0 LANGUAGES C CXX)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(learn_opengl_linux_project
    src/main.cpp
    src/shader.cpp
    src/camera.cpp
    src/stb_image.cpp
    src/glad/glad.c
)

target_include_directories(learn_opengl_linux_project PUBLIC
    ${CMAKE_SOURCE_DIR}/src/include
)

target_link_libraries(learn_opengl_linux_project
    PRIVATE glfw
    PRIVATE glm::glm
)