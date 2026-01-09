cmake_minimum_required(VERSION 3.10)
project(game VERSION 1.0)
cmake_policy(SET CMP0072 NEW)

if(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall")

# Добавляем SOIL2
add_subdirectory(libs/SOIL2)

# Добавляем исполняемый файл
add_executable(game
    src/main.cpp
    src/glad.c
    src/graphics/shader_program.cpp
    src/graphics/renderer.cpp
    src/graphics/camera.cpp
    src/core/engine.cpp
    src/core/input_system.cpp)

# Подключаем заголовочные файлы
target_include_directories(game PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/libs/SOIL2/src
    ${GLM_INCLUDE_DIRS})  # Добавляем GLM

# Линкуем библиотеки
target_link_libraries(game
    glfw
    ${OPENGL_gl_LIBRARY}
    soil2
    dl)  # Добавляем динамическую библиотеку

# macOS специфичные настройки
if(APPLE)
    # Критически важные фреймворки
    target_link_libraries(game
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework CoreVideo")
    
    # Флаги компиляции для macOS
    target_compile_options(game PRIVATE
        -Wno-deprecated-declarations)
endif()

# Копируем ресурсы в директорию сборки
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Устанавливаем рабочую директорию для отладки
set_target_properties(game PROPERTIES
    MACOSX_BUNDLE FALSE
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")