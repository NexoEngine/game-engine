cmake_minimum_required(VERSION 3.30)

# PROJECT
project(client CXX)

set(CMAKE_CXX_STANDARD 20)
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)
set(VCPKG_APPINSTALL_DEPS ON)

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
# SETUP TESTS
enable_testing()
include("${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")

include_directories("./common")

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Installation rules
install(TARGETS nexoEditor
        RUNTIME DESTINATION bin
)

# Automatically install runtime dependencies (Windows .dll files, Linux .so files, etc.)
install(TARGETS nexoEditor
        RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-" ".*system32.*"
        POST_EXCLUDE_REGEXES ".*windows.*"
        RUNTIME DESTINATION bin
)

# Install assets, config, and logs directories
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets/" DESTINATION assets)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/config/" DESTINATION config)
install(DIRECTORY DESTINATION logs)
