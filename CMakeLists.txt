cmake_minimum_required(VERSION 3.10)
project(OpenGL_GLFW_Project)

# Activer le support de C++17
set(CMAKE_CXX_STANDARD 17)

# Rechercher OpenGL
find_package(OpenGL REQUIRED)

# Ajouter les répertoires d'inclusion et de bibliothèque pour GLFW
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/lib")

# Ajouter le répertoire d'inclusion pour GLAD
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/glad")

# Ajouter les fichiers sources
add_executable(${PROJECT_NAME} src/main.cpp src/glad.c
        src/shaderClass.h)

# Inclure les répertoires d'en-tête
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})

# Lier OpenGL et GLFW à l'exécutable
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} "${GLFW_LIB_DIR}/libglfw3.a")

# Copy the shaders directory in the right place during compilation
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
