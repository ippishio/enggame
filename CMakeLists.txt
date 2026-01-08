cmake_minimum_required(VERSION 3.10)
project(game VERSION 1.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#only uncomment if you are going into debug mode in gdb
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(libs/SOIL2)

# Add executable with GLAD source file
add_executable(game
src/main.cpp
src/glad.c
src/graphics/shader_program.cpp
src/graphics/renderer.cpp
src/graphics/camera.cpp) # Add glad.c to your src/ folder


# Include your project's include directory
target_include_directories(game PRIVATE
${PROJECT_SOURCE_DIR}/include
${PROJECT_SOURCE_DIR}/libs/SOIL2/src)

target_include_directories(game PUBLIC ${OPENGL_INCLUDE_DIR})

target_link_libraries(game
glfw
${OPENGL_gl_LIBRARY}
soil2)

if(APPLE)
    target_link_libraries(game "-framework Cocoa")
    target_link_libraries(game "-framework OpenGL")
    target_link_libraries(game "-framework IOKit")
    target_link_libraries(game glfw ${OPENGL_gl_LIBRARY})
endif()