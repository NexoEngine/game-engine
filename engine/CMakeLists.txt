cmake_minimum_required(VERSION 3.10)

# Set project name
project(nexoRenderer VERSION 1.0.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
set(COMMON_SOURCES
        engine/src/Nexo.cpp
        engine/src/external/glad.c
        engine/src/external/stb_image.cpp
        engine/src/Application.cpp
        engine/src/Application.cpp
        engine/src/renderer/Window.cpp
        engine/src/core/event/Event.cpp
        engine/src/core/event/Input.cpp
        engine/src/core/event/opengl/InputOpenGl.cpp
        engine/src/core/event/WindowEvent.cpp
        engine/src/renderer/Buffer.cpp
        engine/src/renderer/Shader.cpp
        engine/src/renderer/VertexArray.cpp
        engine/src/renderer/RendererAPI.cpp
        engine/src/renderer/Renderer.cpp
        engine/src/renderer/RenderCommand.cpp
        engine/src/renderer/Texture.cpp
        engine/src/renderer/SubTexture2D.cpp
        engine/src/renderer/Renderer2D.cpp
        engine/src/renderer/Framebuffer.cpp
        engine/src/core/camera/OrthographicCamera.cpp
        engine/src/core/camera/OrthographicCameraController.cpp
        engine/src/core/camera/Camera.cpp
        engine/src/core/layer/LayerStack.cpp
        engine/src/core/scene/Scene.cpp
        engine/src/core/scene/SceneManager.cpp
        engine/src/core/exceptions/Exceptions.cpp
        engine/src/ecs/Entity.cpp
)

# Add API-specific sources
if(GRAPHICS_API STREQUAL "OpenGL")
    list(APPEND COMMON_SOURCES
            engine/src/renderer/opengl/OpenGlBuffer.cpp
            engine/src/renderer/opengl/OpenGlWindow.cpp
            engine/src/renderer/opengl/OpenGlVertexArray.cpp
            engine/src/renderer/opengl/OpenGlTexture2D.cpp
            engine/src/renderer/opengl/OpenGlShader.cpp
            engine/src/renderer/opengl/OpenGlRendererApi.cpp
            engine/src/renderer/opengl/OpenGlFramebuffer.cpp
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
target_include_directories(nexoRenderer PUBLIC ${CMAKE_SOURCE_DIR}/engine/src)

if(GRAPHICS_API STREQUAL "OpenGL")
    target_compile_definitions(nexoRenderer PRIVATE GRAPHICS_API_OPENGL)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 3.3 REQUIRED)
    target_link_libraries(nexoRenderer PRIVATE OpenGL::GL glfw)
endif()
