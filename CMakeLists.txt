cmake_minimum_required(VERSION 3.10)
project(computerGraphics)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external)



find_package(OpenGL REQUIRED)

set(libs
        ${OPENGL_LIBRARY}
        glfw
        glad
        glm
)

add_executable(${PROJECT_NAME}
#        src/main.cpp
        src/HW/hw002/first_rectangle.cpp
        src/HW/utils/shader_utils/reader.h
        src/HW/utils/shader_utils/reader.cpp
        src/HW/utils/shader_utils/creator.cpp
        src/HW/utils/shader_utils/creator.h)
target_link_libraries(${PROJECT_NAME} ${libs})
