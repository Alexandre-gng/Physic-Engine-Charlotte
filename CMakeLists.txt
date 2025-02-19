cmake_minimum_required(VERSION 3.10)
project(OpenGL_GLFW_Project)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies")
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies/glad")
set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies/glm")

# Ajouter les fichiers sources
add_executable(${PROJECT_NAME}
        src/main.cpp
        src/glad.c

        # engine files
        include/Engine/Shader.hpp
        include/Engine/Camera.hpp
        include/Engine/Renderer.hpp
        include/Engine/Physic.hpp

        # objects files
        include/Objects/Cloth.hpp
        include/Objects/Wall.hpp
        include/Objects/Cube.hpp
        include/Objects/Object.hpp

        # specific constraints files
        include/Constraints/Constraint.hpp
        include/Constraints/RigidBodyColConstraint.hpp
        include/Constraints/StretchingConstraint.hpp

        include/Particle.hpp

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
        src/Cube.cpp
        src/Renderer.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} "${GLFW_LIB_DIR}/libglfw3.a")

