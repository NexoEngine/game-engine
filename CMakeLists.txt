cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "GCC/Clang detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" -Wmissing-field-initializers)
endif()

# PROJECT
project(client CXX)


# SETUP GIT SUBMODULES

find_package(Git QUIET)

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
if(WIN32)
    include("${CMAKE_CURRENT_SOURCE_DIR}\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")
else()
    include("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

# RAYLIB DOWNLOAD
include("${CMAKE_CURRENT_SOURCE_DIR}/external/raylib/CMakeLists.txt")

# ADOBE IMGUI DOWNLOAD
#include("${CMAKE_CURRENT_SOURCE_DIR}/external/adobe_imgui/CMakeLists.txt")

# RLIMGUI DOWNLOAD
include("${CMAKE_CURRENT_SOURCE_DIR}/external/rlImGui/CMakeLists.txt")

# IMGUIZMO DOWNLOAD
include("${CMAKE_CURRENT_SOURCE_DIR}/external/ImGuizmo/CMakeLists.txt")

# SETUP GAME ENGINE GUI
include("${CMAKE_CURRENT_SOURCE_DIR}/src/game_engine/CMakeLists.txt")
