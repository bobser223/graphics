cmake_minimum_required(VERSION 3.10)
project(computerGraphics)

set(CMAKE_CXX_STANDARD 26)

add_subdirectory(external)
include_directories(src/HW/utils/shader_utils)


find_package(OpenGL REQUIRED)

set(libs
        ${OPENGL_LIBRARY}
        glfw
        glad
        glm
)

add_executable(homework002
#        src/main.cpp
        src/HW/hw002/first_rectangle.cpp
        src/HW/utils/shader_utils/reader.h
        src/HW/utils/shader_utils/reader.cpp
        src/HW/utils/shader_utils/creator.cpp
        src/HW/utils/shader_utils/creator.h)

target_link_libraries(homework002 ${libs})

add_executable( homework003
        src/HW/hw003/first_rectangle.cpp
        src/HW/utils/shader_utils/creator.h
        src/HW/utils/shader_utils/creator.cpp
        src/HW/utils/shader_utils/reader.h
        src/HW/utils/shader_utils/reader.cpp

)

target_link_libraries(homework003 ${libs})




