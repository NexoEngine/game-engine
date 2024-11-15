cmake_minimum_required(VERSION 3.17)

# Set project name
project(nexoEditor)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRCS
        common/Exception.cpp
        common/math/Matrix.cpp
        editor/main.cpp
        editor/external/imgui_impl_opengl3.cpp
        editor/external/imgui_impl_glfw.cpp
        editor/src/backends/ImGuiBackend.cpp
        editor/src/backends/opengl/openglImGuiBackend.cpp
        editor/src/Editor.cpp
        editor/src/SceneManagerBridge.cpp
        editor/src/DocumentWindows/ConsoleWindow.cpp
        editor/src/DocumentWindows/MainScene.cpp
        editor/src/DocumentWindows/SceneTreeWindow.cpp
        editor/src/DocumentWindows/PopupManager.cpp
        editor/src/DocumentWindows/SceneViewManager.cpp
)

add_executable(nexoEditor ${SRCS})

set_target_properties(nexoEditor
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
)

include_directories("./editor/src")
include_directories("./engine/src")
include_directories("./editor/external")
include_directories("./common")

if(WIN32)
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/include")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/bin")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-windows/include")
elseif(UNIX AND NOT APPLE)
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-linux/include")
elseif(APPLE)
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-osx/include")
endif()

if(APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
endif()

# LOGURU
if (WIN32)
    find_path(LOGURU_INCLUDE_DIRS "loguru/loguru.cpp")
    target_include_directories(nexoEditor PRIVATE ${LOGURU_INCLUDE_DIRS})
    target_sources(nexoEditor PRIVATE ${LOGURU_INCLUDE_DIRS}/loguru/loguru.cpp)
else()
    find_package(loguru CONFIG REQUIRED)
    target_link_libraries(nexoEditor PRIVATE loguru)
endif()

# ImGuizmo
find_package(imguizmo CONFIG QUIET)
target_link_libraries(nexoEditor PRIVATE imguizmo)

# ImGui
target_link_libraries(nexoEditor PRIVATE nexoRenderer imgui::imgui)

# TinyFileDialogs
find_package(tinyfiledialogs CONFIG REQUIRED)
target_link_libraries(nexoEditor PRIVATE tinyfiledialogs::tinyfiledialogs)

include_directories(include)

if(GRAPHICS_API STREQUAL "OpenGL")
    target_compile_definitions(nexoEditor PRIVATE GRAPHICS_API_OPENGL)
endif()
