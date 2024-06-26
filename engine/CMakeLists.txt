# This cmake file is modified version based on the examples found in the link below.
#   - https://github.com/ttroy50/cmake-examples


cmake_minimum_required(VERSION 3.5)

# Set the project name
project(engine)

set(SOURCES
    OpenGL/src/renderer.cpp
    OpenGL/src/shader.cpp
    OpenGL/src/window.cpp
    OpenGL/src/texture.cpp
    OpenGL/vendor/glad.c
    util/logger.cpp
    util/util.cpp
)

# Add a library with the above sources
add_library(${PROJECT_NAME} ${SOURCES})
# add_library(sub::lib1 ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/OpenGL/vendor/include
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/OpenGL/src/include
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/util
)

# Set the directories that should be linked in the build command for this target
# when running the compiler these will be included as -L/directory/path/
if (${CMAKE_HOST_SYSTEM_NAME} EQUAL "Windows")
    # Windows only
    target_link_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/OpenGL/vendor/lib/glfw-win)
    target_link_libraries(${PROJECT_NAME} PUBLIC glew32 glfw3 opengl32 glu32 gdi32)
else()
    # Linux only
    target_link_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/OpenGL/vendor/lib/glfw-linux)
    target_link_libraries(${PROJECT_NAME} PUBLIC libglfw3.a GL m X11 pthread Xi Xrandr dl)
endif()
