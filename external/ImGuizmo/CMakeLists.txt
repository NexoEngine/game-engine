cmake_minimum_required(VERSION 3.17)

# Include FetchContent module
include(FetchContent)

# Declare ImGuizmo dependency
FetchContent_Declare(
        imguizmo
        GIT_REPOSITORY https://github.com/CedricGuillemet/ImGuizmo.git
        GIT_TAG ba662b119d64f9ab700bb2cd7b2781f9044f5565
)

# Fetch the content at configure time
FetchContent_MakeAvailable(imguizmo)

# Find imgui package
find_package(imgui QUIET)
include_directories(${imgui_INCLUDE_DIRS})

# Define ImGuizmo library
add_library(imguizmo STATIC
            ${imguizmo_SOURCE_DIR}/GraphEditor.cpp
            ${imguizmo_SOURCE_DIR}/ImCurveEdit.cpp
            ${imguizmo_SOURCE_DIR}/ImGradient.cpp
            ${imguizmo_SOURCE_DIR}/ImGuizmo.cpp
            ${imguizmo_SOURCE_DIR}/ImSequencer.cpp
)

target_compile_options(imguizmo PRIVATE "-std=c++11")

# Include directories for ImGuizmo
target_include_directories(imguizmo PUBLIC
                           $<BUILD_INTERFACE:${imguizmo_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:include>
                           PRIVATE ${IMGUI_INCLUDE_DIRS}
)

# Link ImGui to ImGuizmo
target_link_libraries(imguizmo PRIVATE imgui::imgui)
