cmake_minimum_required(VERSION 3.10)
project(OpenGL_GLFW_Project)

# Activer le support de C++17
set(CMAKE_CXX_STANDARD 17)

# Rechercher OpenGL
find_package(OpenGL REQUIRED)

# Adding dependencies directories
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies")
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies/glad")
set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies/glm")

# Ajouter les fichiers sources
add_executable(${PROJECT_NAME}
        src/main.cpp
        src/glad.c
        include/OPENGL/Shader.hpp
        include/OPENGL/Camera.hpp

        include/Objects/Cloth.hpp
        include/Objects/Wall.hpp
        include/Objects/Object.hpp

        include/Constraints/Constraint.hpp
        include/Constraints/RigidBodyColConstraint.hpp
        include/Constraints/StretchingConstraint.hpp

        include/Particle.hpp
        include/Physic.hpp

        include/common.hpp
        include/Joint.hpp
        include/Triangle.hpp
        src/Cloth.cpp
        src/Joint.cpp
        src/Object.cpp
        src/Particle.cpp
        src/Physic.cpp
        src/RigidBodyColConstraint.cpp
        src/StretchingConstraint.cpp
        include/OPENGL/Renderer.hpp
        include/Objects/Cube.hpp
        src/Cube.cpp
        src/Renderer.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} "${GLFW_LIB_DIR}/libglfw3.a")

