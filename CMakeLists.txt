# Set CMake version
cmake_minimum_required(VERSION 3.22)

# Project details
set(PROJECT_NAME sdl2-tutorials)
project(${PROJECT_NAME} LANGUAGES CXX)

# Set source directory
aux_source_directory(./src SRC_LIST)

# Disable in-source builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

# Set the default build type to debug, unless otherwise stated
if (NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE Debug)
endif()

# Set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG    ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG    ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG    ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_BINARY_DIR}/lib)

# Define the executable
add_executable(${PROJECT_NAME})

# Add the source files
add_subdirectory(src)

# Add 3rd party libraries
add_subdirectory(lib)

# Set compile flags.
target_compile_options(${PROJECT_NAME} PRIVATE
    # Clang
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Weverything>
    # GCC
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -fdiagnostics-color=always>
    # Visual Studio
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Find dependencies
find_package(OpenGL COMPONENTS OpenGL REQUIRED)

# Include and link against dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL SDL2-static)
