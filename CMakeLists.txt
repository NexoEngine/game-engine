cmake_minimum_required(VERSION 3.28)

# PROJECT
project(client CXX)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "GCC/Clang detected, adding compile flags")
    add_compile_options(--std=c++20 -g -Wmissing-field-initializers -Wall -Wextra -Wpedantic)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-fexperimental-library)
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message(STATUS "MSVC detected, adding compile flags")
    add_compile_options(/std:c++20 /W4 /Zi /Zc:preprocessor)
else()
    message(FATAL_ERROR "Unsupported compiler ${CMAKE_CXX_COMPILER_ID}")
endif()

set(GRAPHICS_API "OpenGL")

# SETUP GIT SUBMODULES

find_package(Git QUIET)

add_definitions(-DNO_RAYLIB_COLORS)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule sync --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule sync --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --remote
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

# SETUP VCPKG

if (WIN32)
    execute_process(
            COMMAND .\\vcpkg\\bootstrap-vcpkg.bat
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
else()
    execute_process(
            COMMAND ./vcpkg/bootstrap-vcpkg.sh
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

# RUNNING VCPKG
include("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

# SETUP EDITOR
include("${CMAKE_CURRENT_SOURCE_DIR}/editor/CMakeLists.txt")
# SETUP ENGINE
include("${CMAKE_CURRENT_SOURCE_DIR}/engine/CMakeLists.txt")
# SETUP EXAMPLE
include("${CMAKE_CURRENT_SOURCE_DIR}/examples/ecs/CMakeLists.txt")
# SETUP TESTS
enable_testing()
include("${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")

include_directories("./common")

include("${CMAKE_CURRENT_SOURCE_DIR}/scripts/install.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/scripts/pack.cmake")
