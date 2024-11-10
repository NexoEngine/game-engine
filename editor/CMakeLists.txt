cmake_minimum_required(VERSION 3.17)

# Set project name
project(nexoEditor)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRCS
        editor/main.cpp
        editor/external/imgui_impl_opengl3.cpp
        editor/external/imgui_impl_glfw.cpp
        editor/src/backends/ImGuiBackend.cpp
        editor/src/backends/opengl/openglImGuiBackend.cpp
        editor/src/Editor.cpp
        editor/src/SceneManagerBridge.cpp
        editor/src/DocumentWindows/ConsoleWindow.cpp
)

FetchContent_Declare(
        tinyfiledialogs
        GIT_REPOSITORY https://github.com/native-toolkit/libtinyfiledialogs.git
        GIT_TAG master
)
FetchContent_MakeAvailable(tinyfiledialogs)

list(APPEND MY_PROJECT_SOURCES ${tinyfiledialogs_SOURCE_DIR}/tinyfiledialogs.c)

add_executable(nexoEditor ${SRCS} ${tinyfiledialogs_SOURCE_DIR}/tinyfiledialogs.c)

set_target_properties( nexoEditor
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
)

include_directories("./editor/src")
include_directories("./engine/src")
include_directories("./editor/external")

if(WIN32)
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/include")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/bin")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-windows/include")
elseif(UNIX AND NOT APPLE)
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-subbuild")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-build/raylib/include")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-linux/include")
elseif(APPLE)
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-subbuild")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-build/raylib/include")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-osx/include")
endif()

if(APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra --std=c++20 -O3 -g")
endif()

find_package(raylib 5.0 CONFIG QUIET)
include_directories(${raylib_INCLUDE_DIRS})

if (WIN32)
    find_path(LOGURU_INCLUDE_DIRS "loguru/loguru.cpp")
    target_include_directories(nexoEditor PRIVATE ${LOGURU_INCLUDE_DIRS})
    target_sources(nexoEditor PRIVATE ${LOGURU_INCLUDE_DIRS}/loguru/loguru.cpp)
else()
    find_package(loguru CONFIG REQUIRED)
    target_link_libraries(nexoEditor PRIVATE loguru)
endif()

find_package(imguizmo CONFIG QUIET)
target_link_libraries(nexoEditor PRIVATE imguizmo)

target_link_libraries(nexoEditor PRIVATE nexoRenderer raylib imgui::imgui)

target_include_directories(nexoEditor PRIVATE ${tinyfiledialogs_SOURCE_DIR})

if(UNIX)
    set(CMAKE_CXX_FLAGS "--std=c++20 -Wall -Wextra -O3 -g")
endif()

include_directories(include)

if(GRAPHICS_API STREQUAL "OpenGL")
    target_compile_definitions(nexoEditor PRIVATE GRAPHICS_API_OPENGL)
endif()
