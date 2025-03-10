cmake_minimum_required(VERSION 3.10)

# Set project name
project(nexoRenderer VERSION 1.0.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
set(COMMON_SOURCES
        common/Exception.cpp
        common/math/Vector.cpp
        common/math/Projection.cpp
        engine/src/Nexo.cpp
        engine/src/EntityFactory3D.cpp
        engine/src/LightFactory.cpp
        engine/src/CameraFactory.cpp
        engine/src/Application.cpp
        engine/src/renderer/Window.cpp
        engine/src/core/event/Event.cpp
        engine/src/core/event/Input.cpp
        engine/src/core/event/SignalEvent.cpp
        engine/src/core/event/opengl/InputOpenGl.cpp
        engine/src/core/event/WindowEvent.cpp
        engine/src/components/Camera.cpp
        engine/src/renderer/Buffer.cpp
        engine/src/renderer/Shader.cpp
        engine/src/renderer/ShaderLibrary.cpp
        engine/src/renderer/ShaderStorageBuffer.cpp
        engine/src/renderer/VertexArray.cpp
        engine/src/renderer/RendererAPI.cpp
        engine/src/renderer/Renderer.cpp
        engine/src/renderer/RenderCommand.cpp
        engine/src/renderer/Texture.cpp
        engine/src/renderer/SubTexture2D.cpp
        engine/src/renderer/Renderer3D.cpp
        engine/src/renderer/Framebuffer.cpp
        engine/src/renderer/UniformCache.cpp
        engine/src/renderer/DrawCommand.cpp
        engine/src/renderer/RenderPipeline.cpp
        engine/src/renderer/primitives/Cube.cpp
        engine/src/renderer/primitives/Billboard.cpp
        engine/src/renderer/primitives/Tetrahedron.cpp
        engine/src/renderer/primitives/Pyramid.cpp
        engine/src/renderer/primitives/Cylinder.cpp
        engine/src/renderer/primitives/Sphere.cpp
        engine/src/core/scene/Scene.cpp
        engine/src/core/scene/SceneManager.cpp
        engine/src/ecs/Entity.cpp
        engine/src/ecs/Components.cpp
        engine/src/ecs/ComponentArray.cpp
        engine/src/ecs/Coordinator.cpp
        engine/src/ecs/System.cpp
        engine/src/systems/CameraSystem.cpp
        engine/src/systems/RenderCommandSystem.cpp
        engine/src/systems/RenderBillboardSystem.cpp
        engine/src/systems/LightSystem.cpp
        engine/src/systems/OnSceneDeletedSystem.cpp
        engine/src/systems/PhysicsSystem.cpp
        engine/src/systems/ScriptingSystem.cpp
        engine/src/systems/lights/AmbientLightSystem.cpp
        engine/src/systems/lights/PointLightsSystem.cpp
        engine/src/systems/lights/DirectionalLightsSystem.cpp
        engine/src/systems/lights/SpotLightsSystem.cpp
        engine/src/systems/TransformHierarchySystem.cpp
        engine/src/systems/TransformMatrixSystem.cpp
        engine/src/renderPasses/ForwardPass.cpp
        engine/src/renderPasses/GridPass.cpp
        engine/src/renderPasses/MaskPass.cpp
        engine/src/renderPasses/OutlinePass.cpp
        engine/src/assets/Asset.cpp
        engine/src/assets/AssetRef.cpp
        engine/src/assets/AssetLocation.cpp
        engine/src/assets/AssetCatalog.cpp
        engine/src/assets/AssetImporter.cpp
        engine/src/assets/AssetImporterContext.cpp
        engine/src/assets/Assets/Model/ModelImporter.cpp
        engine/src/assets/Assets/Texture/TextureImporter.cpp
        engine/src/scripting/native/Scripting.cpp
        engine/src/scripting/native/HostString.cpp
        engine/src/scripting/native/NativeApi.cpp
)

# Add API-specific sources
if(NEXO_GRAPHICS_API STREQUAL "OpenGL")
    list(APPEND COMMON_SOURCES
            engine/src/renderer/opengl/OpenGlBuffer.cpp
            engine/src/renderer/opengl/OpenGlWindow.cpp
            engine/src/renderer/opengl/OpenGlVertexArray.cpp
            engine/src/renderer/opengl/OpenGlTexture2D.cpp
            engine/src/renderer/opengl/OpenGlShader.cpp
            engine/src/renderer/opengl/OpenGlShaderStorageBuffer.cpp
            engine/src/renderer/opengl/OpenGlRendererApi.cpp
            engine/src/renderer/opengl/OpenGlFramebuffer.cpp
            engine/src/renderer/opengl/OpenGlShaderReflection.cpp
    )
endif()

# Create the library
add_library(nexoRenderer STATIC ${COMMON_SOURCES})

# Find glm and add its include directories
find_package(glm CONFIG REQUIRED)

if(glm_FOUND)
    message(STATUS "GLM found at: ${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-linux/include")
    target_include_directories(nexoRenderer PUBLIC ${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-linux/include)
else()
    message(FATAL_ERROR "GLM not found!")
endif()

# Add include directories for this target
target_include_directories(nexoRenderer PUBLIC
                           ${CMAKE_SOURCE_DIR}/engine/include
                           ${CMAKE_SOURCE_DIR}/engine/src
                           ${CMAKE_SOURCE_DIR}/common)

# loguru
find_package(loguru CONFIG REQUIRED)
target_link_libraries(nexoRenderer PRIVATE loguru::loguru)

# Stb
find_package(Stb REQUIRED)
target_include_directories(nexoRenderer PRIVATE ${Stb_INCLUDE_DIR})
target_sources(nexoRenderer PRIVATE ${CMAKE_SOURCE_DIR}/engine/external/stb_image.cpp)

# assimp
find_package(assimp CONFIG REQUIRED)
target_link_libraries(nexoRenderer PRIVATE assimp::assimp)

# boost-dll
find_package(Boost CONFIG REQUIRED COMPONENTS dll)
target_link_libraries(nexoRenderer PRIVATE Boost::dll)

# joltphysics
find_package(Jolt CONFIG REQUIRED)
target_link_libraries(nexoRenderer PRIVATE Jolt::Jolt)

###########################################
# Scripting
###########################################

# nethost
find_package(unofficial-nethost CONFIG REQUIRED)
target_link_libraries(nexoRenderer PRIVATE unofficial::nethost::nethost)

if(NEXO_GRAPHICS_API STREQUAL "OpenGL")
    target_compile_definitions(nexoRenderer PRIVATE NX_GRAPHICS_API_OPENGL)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 3.4 REQUIRED)
    find_package(glad CONFIG REQUIRED)
    target_link_libraries(nexoRenderer PRIVATE OpenGL::GL glfw glad::glad)
endif()

target_compile_definitions(nexoRenderer PRIVATE NEXO_EXPORT)
set_target_properties(nexoRenderer PROPERTIES ENABLE_EXPORTS ON)
