# This cmake file is modified version based on the one found in the link below.
#   - https://github.com/ttroy50/cmake-examples/tree/master/01-basic/B-hello-headers

# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message("Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# TODO: make this change based on the OS
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_STANDARD 14)

# Set the project name
project (velocity_cpp)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/main.cpp
    src/logger.cpp
    vendor/glad.c
)

# Add an executable with the above sources
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/include)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/vendor/include)

# Set the directories that should be linked in the build command for this target
# when running g++ these will be included as -L/directory/path/

if (${CMAKE_HOST_SYSTEM_NAME} EQUAL "Windows")
    # Windows only
    target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/vendor/lib/glfw-win)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glew32 glfw3 opengl32 glu32 gdi32)
else()
    # Linux only
    target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/vendor/lib/glfw-linux)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw3 GL m X11 pthread Xi Xrandr dl)
endif()
