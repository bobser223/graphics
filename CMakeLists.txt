cmake_minimum_required(VERSION 3.10)
project(computerGraphics)

set(CMAKE_CXX_STANDARD 26)

add_subdirectory(external)
include_directories(src/HW/utils/shader_utils)
include_directories(src/hw/utils/texture_utils)


find_package(OpenGL REQUIRED)

set(libs
        ${OPENGL_LIBRARY}
        glfw
        glad
        glm
        stb
)

add_executable(homework002
#        src/main.cpp
        src/HW/hw002/first_rectangle.cpp
        src/HW/utils/shader_utils/reader.h
        src/HW/utils/shader_utils/reader.cpp
        src/HW/utils/shader_utils/creator.cpp
        src/HW/utils/shader_utils/creator.h
        src/HW/utils/texture_utils/texture_creator.h
        src/HW/utils/texture_utils/texture_creator.cpp
        src/HW/hw004/rendering_cube.cpp)

target_link_libraries(homework002 ${libs})

add_executable( homework003
        src/HW/hw003/three_rectangles.cpp
        src/HW/utils/shader_utils/creator.h
        src/HW/utils/shader_utils/creator.cpp
        src/HW/utils/shader_utils/reader.h
        src/HW/utils/shader_utils/reader.cpp
        src/HW/utils/texture_utils/texture_creator.h
        src/HW/utils/texture_utils/texture_creator.cpp
)

target_link_libraries(homework003 ${libs})


add_executable(homework003_rotation
        src/HW/hw003/rotation/rotation.cpp
        src/HW/utils/shader_utils/creator.h
        src/HW/utils/shader_utils/creator.cpp
        src/HW/utils/shader_utils/reader.h
        src/HW/utils/shader_utils/reader.cpp
        src/HW/utils/texture_utils/texture_creator.h
        src/HW/utils/texture_utils/texture_creator.cpp
)

target_link_libraries(homework003_rotation ${libs})

add_executable(homework004_cube
        src/HW/hw004/rendering_cube.cpp
        src/HW/utils/shader_utils/creator.h
        src/HW/utils/shader_utils/creator.cpp
        src/HW/utils/shader_utils/reader.h
        src/HW/utils/shader_utils/reader.cpp
        src/HW/utils/texture_utils/texture_creator.h
        src/HW/utils/texture_utils/texture_creator.cpp
)

target_link_libraries(homework004_cube ${libs})




