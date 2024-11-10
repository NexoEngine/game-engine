include(FetchContent)

# Fetch Adobe ImGui (docking branch)
FetchContent_Declare(
        adobe_imgui
        GIT_REPOSITORY https://github.com/adobe/imgui.git
        GIT_TAG docking  # Ensure you are using the docking branch
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(adobe_imgui)

# Fetch Official ImGui for Backends
FetchContent_Declare(
        imgui_official
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG master  # Use the latest master branch
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(imgui_official)
