#### CMakeLists.txt ###########################################################
#
#  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
#  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
#  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
#  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
#  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
#
#  Author:      Mehdy MORVAN
#  Date:        22/11/2024
#  Description: CMakeLists.txt file for the renderer tests.
#
###############################################################################

cmake_minimum_required(VERSION 3.10)

project(rendererTests)

set(BASEDIR ${CMAKE_CURRENT_LIST_DIR})

include_directories("./common")
include_directories("./engine/src/renderer")

# TODO: make common a library and link it to the tests
set(COMMON_SOURCES
        common/Exception.cpp
        common/math/Matrix.cpp
)

set(RENDERER_SOURCES
        engine/src/renderer/Window.cpp
        engine/src/renderer/Buffer.cpp
        engine/src/renderer/Shader.cpp
        engine/src/renderer/VertexArray.cpp
        engine/src/renderer/RendererAPI.cpp
        engine/src/renderer/Renderer.cpp
        engine/src/renderer/RenderCommand.cpp
        engine/src/renderer/Texture.cpp
        engine/src/renderer/SubTexture2D.cpp
        engine/src/renderer/Renderer2D.cpp
        engine/src/renderer/Renderer3D.cpp
        engine/src/renderer/Framebuffer.cpp
        engine/src/renderer/opengl/OpenGlBuffer.cpp
        engine/src/renderer/opengl/OpenGlWindow.cpp
        engine/src/renderer/opengl/OpenGlVertexArray.cpp
        engine/src/renderer/opengl/OpenGlTexture2D.cpp
        engine/src/renderer/opengl/OpenGlShader.cpp
        engine/src/renderer/opengl/OpenGlRendererApi.cpp
        engine/src/renderer/opengl/OpenGlFramebuffer.cpp
)

add_executable(renderer_tests
        ${COMMON_SOURCES}
        ${RENDERER_SOURCES}
        ${BASEDIR}/Buffer.test.cpp
        ${BASEDIR}/VertexArray.test.cpp
        ${BASEDIR}/Framebuffer.test.cpp
        ${BASEDIR}/Shader.test.cpp
        ${BASEDIR}/RendererAPI.test.cpp
)

# Find glm and add its include directories
find_package(glm CONFIG REQUIRED)
target_include_directories(renderer_tests PUBLIC ${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-linux/include)

find_package(Stb REQUIRED)
target_include_directories(renderer_tests PRIVATE ${Stb_INCLUDE_DIR})
target_sources(renderer_tests PRIVATE ${CMAKE_SOURCE_DIR}/engine/external/stb_image.cpp)

target_compile_definitions(renderer_tests PRIVATE GRAPHICS_API_OPENGL)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glad CONFIG REQUIRED)

# boost-dll
find_package(Boost CONFIG REQUIRED COMPONENTS dll)
target_link_libraries(renderer_tests PRIVATE Boost::dll)

#target_link_libraries(renderer_tests PRIVATE OpenGL::GL glfw glad::glad)

# Link gtest and engine (renderer) libraries
target_link_libraries(renderer_tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock OpenGL::GL glfw glad::glad)
