cmake_minimum_required(VERSION 3.28)

# PROJECT
project(client CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(NEXO_COVERAGE OFF CACHE BOOL "Enable coverage for binaries")
set(NEXO_GIT_SUBMODULE OFF CACHE BOOL "Enable git submodules init and update")
set(NEXO_BOOTSTRAP_VCPKG OFF CACHE BOOL "Enable vcpkg bootstrap")
set(NEXO_BUILD_TESTS ON CACHE BOOL "Enable tests")
set(NEXO_BUILD_EXAMPLES OFF CACHE BOOL "Enable examples")
set(NEXO_GRAPHICS_API "OpenGL" CACHE STRING "Graphics API to use")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(NEXO_COMPILER_FLAGS_ALL --std=c++${CMAKE_CXX_STANDARD})
    set(NEXO_COMPILER_FLAGS_DEBUG -g -Wmissing-field-initializers -Wall -Wextra -Wpedantic)
    set(NEXO_COMPILER_FLAGS_RELEASE -O3)
    set(NEXO_COVERAGE_FLAGS -O0 --coverage)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(NEXO_COMPILER_FLAGS_ALL /std:c++${CMAKE_CXX_STANDARD} /Zc:preprocessor)
    set(NEXO_COMPILER_FLAGS_DEBUG /Zi /Od /Zc:preprocessor)
    set(NEXO_COMPILER_FLAGS_RELEASE /O2 /Zc:preprocessor)
    set(NEXO_COVERAGE_FLAGS "")  # MSVC doesn't support coverage in the same way
else()
    message(WARNING "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}, using default flags")
endif()

add_compile_options(
        "${NEXO_COMPILER_FLAGS_ALL}"
        "$<$<CONFIG:Debug>:${NEXO_COMPILER_FLAGS_DEBUG}>"
        "$<$<CONFIG:Release>:${NEXO_COMPILER_FLAGS_RELEASE}>"
)

# Prevent Visual Studio (or other build tools) from creating per config sub-directories (e.g. Debug, Release)
# Useful to look for resource files relative to the executable path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}>)

if (NEXO_COVERAGE)
    message(STATUS "Coverage enabled, adding flags: ${NEXO_COVERAGE_FLAGS}")
    add_compile_options("$<$<CONFIG:Debug>:${NEXO_COVERAGE_FLAGS}>")
    add_link_options("$<$<CONFIG:Debug>:${NEXO_COVERAGE_FLAGS}>")
endif()

# SETUP GIT SUBMODULES
if (NEXO_GIT_SUBMODULE)
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
endif()

# SETUP VCPKG
if(NEXO_BOOTSTRAP_VCPKG)
    message(STATUS "Bootstraping VCPKG")
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
else()
    message(STATUS "Skipping VCPKG bootstrap")
endif()

# RUNNING VCPKG
message(STATUS "Running VCPKG...")
include("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
message(STATUS "VCPKG done.")

# SETUP EDITOR
include("${CMAKE_CURRENT_SOURCE_DIR}/editor/CMakeLists.txt")
# SETUP ENGINE
include("${CMAKE_CURRENT_SOURCE_DIR}/engine/CMakeLists.txt")
# SETUP MANAGED CSHARP LIB
include("${CMAKE_CURRENT_SOURCE_DIR}/engine/src/scripting/managed/CMakeLists.txt")
add_dependencies(nexoEditor nexoManaged)
# SETUP EXAMPLE
include("${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt")
# SETUP TESTS
enable_testing()
include("${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")

include_directories("./common")

include("${CMAKE_CURRENT_SOURCE_DIR}/scripts/pack.cmake")
