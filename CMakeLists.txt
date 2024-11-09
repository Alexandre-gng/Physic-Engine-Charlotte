cmake_minimum_required(VERSION 3.10)
project(OpenGL_GLFW_Project)

# Activer le support de C++17
set(CMAKE_CXX_STANDARD 17)

# Rechercher OpenGL
find_package(OpenGL REQUIRED)

# Adding include directories
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/glad")
set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/glm")

# Ajouter les fichiers sources
add_executable(${PROJECT_NAME}
        src/main.cpp
        src/glad.c
        src/shaderClass.h
        src/cameraClass.h
        src/physicClass.h
        src/particleClass.h)

target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} "${GLFW_LIB_DIR}/libglfw3.a")

