cmake_minimum_required(VERSION 3.17)

# Set project name
project(nexoEditor)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRCS
        common/Exception.cpp
        common/math/Matrix.cpp
        common/math/Light.cpp
        editor/main.cpp
        editor/src/backends/ImGuiBackend.cpp
        editor/src/backends/opengl/openglImGuiBackend.cpp
        editor/src/context/ThumbnailCache.cpp
        editor/src/context/Selector.cpp
        editor/src/context/ActionManager.cpp
        editor/src/context/ActionHistory.cpp
        editor/src/context/ActionGroup.cpp
        editor/src/context/actions/EntityActions.cpp
        editor/src/context/actions/ComponentRestoreFactory.cpp
        editor/src/ImNexo/EntityProperties.cpp
        editor/src/ImNexo/Components.cpp
        editor/src/ImNexo/Elements.cpp
        editor/src/ImNexo/Panels.cpp
        editor/src/ImNexo/Utils.cpp
        editor/src/ImNexo/Widgets.cpp
        editor/src/ImNexo/ImNexo.cpp
        editor/src/utils/ScenePreview.cpp
        editor/src/utils/Config.cpp
        editor/src/utils/String.cpp
        editor/src/utils/FileSystem.cpp
        editor/src/utils/EditorProps.cpp
        editor/src/inputs/Command.cpp
        editor/src/inputs/InputManager.cpp
        editor/src/inputs/WindowState.cpp
        editor/src/Editor.cpp
        editor/src/WindowRegistry.cpp
        editor/src/DockingRegistry.cpp
        editor/src/ADocumentWindow.cpp
        editor/src/DocumentWindows/EditorScene/Gizmo.cpp
        editor/src/DocumentWindows/EditorScene/Init.cpp
        editor/src/DocumentWindows/EditorScene/Shortcuts.cpp
        editor/src/DocumentWindows/EditorScene/Show.cpp
        editor/src/DocumentWindows/EditorScene/Shutdown.cpp
        editor/src/DocumentWindows/EditorScene/Toolbar.cpp
        editor/src/DocumentWindows/EditorScene/Update.cpp
        editor/src/DocumentWindows/EditorScene/DragDrop.cpp
        editor/src/DocumentWindows/AssetManager/Init.cpp
        editor/src/DocumentWindows/AssetManager/Show.cpp
        editor/src/DocumentWindows/AssetManager/Shutdown.cpp
        editor/src/DocumentWindows/AssetManager/Update.cpp
        editor/src/DocumentWindows/AssetManager/FolderTree.cpp
        editor/src/DocumentWindows/AssetManager/Thumbnail.cpp
        editor/src/DocumentWindows/ConsoleWindow/Init.cpp
        editor/src/DocumentWindows/ConsoleWindow/Log.cpp
        editor/src/DocumentWindows/ConsoleWindow/Show.cpp
        editor/src/DocumentWindows/ConsoleWindow/Shutdown.cpp
        editor/src/DocumentWindows/ConsoleWindow/Update.cpp
        editor/src/DocumentWindows/ConsoleWindow/Utils.cpp
        editor/src/DocumentWindows/InspectorWindow/Init.cpp
        editor/src/DocumentWindows/InspectorWindow/Show.cpp
        editor/src/DocumentWindows/InspectorWindow/Shutdown.cpp
        editor/src/DocumentWindows/InspectorWindow/Update.cpp
        editor/src/DocumentWindows/MaterialInspector/Init.cpp
        editor/src/DocumentWindows/MaterialInspector/Show.cpp
        editor/src/DocumentWindows/MaterialInspector/Shutdown.cpp
        editor/src/DocumentWindows/MaterialInspector/Update.cpp
        editor/src/DocumentWindows/SceneTreeWindow/Hovering.cpp
        editor/src/DocumentWindows/SceneTreeWindow/Init.cpp
        editor/src/DocumentWindows/SceneTreeWindow/NodeHandling.cpp
        editor/src/DocumentWindows/SceneTreeWindow/Rename.cpp
        editor/src/DocumentWindows/SceneTreeWindow/SceneCreation.cpp
        editor/src/DocumentWindows/SceneTreeWindow/Selection.cpp
        editor/src/DocumentWindows/SceneTreeWindow/Show.cpp
        editor/src/DocumentWindows/SceneTreeWindow/Shutdown.cpp
        editor/src/DocumentWindows/SceneTreeWindow/Update.cpp
        editor/src/DocumentWindows/SceneTreeWindow/Shortcuts.cpp
        editor/src/DocumentWindows/PopupManager.cpp
        editor/src/DocumentWindows/EntityProperties/TransformProperty.cpp
        editor/src/DocumentWindows/EntityProperties/RenderProperty.cpp
        editor/src/DocumentWindows/EntityProperties/AmbientLightProperty.cpp
        editor/src/DocumentWindows/EntityProperties/DirectionalLightProperty.cpp
        editor/src/DocumentWindows/EntityProperties/PointLightProperty.cpp
        editor/src/DocumentWindows/EntityProperties/SpotLightProperty.cpp
        editor/src/DocumentWindows/EntityProperties/CameraProperty.cpp
        editor/src/DocumentWindows/EntityProperties/CameraController.cpp
        editor/src/DocumentWindows/EntityProperties/CameraTarget.cpp
        editor/src/DocumentWindows/EntityProperties/TypeErasedProperty.cpp
        editor/src/DocumentWindows/EntityProperties/MaterialProperty.cpp
        editor/src/DocumentWindows/TestWindow/Init.cpp
        editor/src/DocumentWindows/TestWindow/Parser.cpp
        editor/src/DocumentWindows/TestWindow/Show.cpp
        editor/src/DocumentWindows/TestWindow/Shutdown.cpp
        editor/src/DocumentWindows/TestWindow/Update.cpp
        editor/src/DocumentWindows/PrimitiveWindow/Init.cpp
        editor/src/DocumentWindows/PrimitiveWindow/Show.cpp
        editor/src/DocumentWindows/PrimitiveWindow/Shutdown.cpp
        editor/src/DocumentWindows/PrimitiveWindow/Update.cpp
        editor/src/DocumentWindows/GameWindow/GameWindow.cpp
        editor/src/DocumentWindows/GameWindow/Setup.cpp
        editor/src/DocumentWindows/GameWindow/Show.cpp
        editor/src/DocumentWindows/GameWindow/Update.cpp
)

# Windows App Icon
if (WIN32)
    list(APPEND SRCS ${CMAKE_SOURCE_DIR}/resources/nexo.rc)
endif()

add_executable(nexoEditor ${SRCS})
set(WAYLAND_APP_ID "app.nexo-engine.editor")
# Add custom DEFINE for the app id
add_compile_definitions(WAYLAND_APP_ID="${WAYLAND_APP_ID}")

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

# loguru
find_package(loguru CONFIG REQUIRED)
target_link_libraries(nexoEditor PRIVATE loguru::loguru)

# ImGuizmo
find_package(imguizmo CONFIG REQUIRED)
target_link_libraries(nexoEditor PRIVATE imguizmo::imguizmo)

# ImGui
find_package(imgui CONFIG REQUIRED)
target_link_libraries(nexoEditor PRIVATE nexoRenderer imgui::imgui)

# TinyFileDialogs
find_package(tinyfiledialogs CONFIG REQUIRED)
target_link_libraries(nexoEditor PRIVATE tinyfiledialogs::tinyfiledialogs)

# boost-dll
find_package(Boost CONFIG REQUIRED COMPONENTS dll)
target_link_libraries(nexoEditor PRIVATE Boost::dll)

# Boost UUID
find_package(boost_uuid CONFIG REQUIRED)
target_link_libraries(nexoEditor PRIVATE Boost::uuid)

include_directories(include)

if(NEXO_GRAPHICS_API STREQUAL "OpenGL")
    target_compile_definitions(nexoEditor PRIVATE NX_GRAPHICS_API_OPENGL)
endif()

# Set the output directory for the executable (prevents generator from creating Debug/Release folders)
set_target_properties(nexoEditor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<0:>)
