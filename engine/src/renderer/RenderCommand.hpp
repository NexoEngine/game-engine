//// RenderCommand.hpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for render command class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "RendererAPI.hpp"

namespace nexo::renderer {
    /**
     * @class RenderCommand
     * @brief Provides a high-level interface for issuing rendering commands.
     *
     * The `RenderCommand` class serves as an abstraction layer over the graphics API
     * (e.g., OpenGL, DirectX, Vulkan), allowing the application to issue rendering
     * commands without being tightly coupled to a specific API.
     *
     * Features:
     * - Initialization and management of the renderer API.
     * - High-level rendering commands such as setting the viewport, clearing the screen,
     *   and drawing indexed primitives.
     *
     * Responsibilities:
     * - Delegates rendering operations to the underlying `RendererApi` implementation.
     * - Provides static methods for commonly used rendering commands.
     *
     * Usage:
     * - Before issuing any rendering commands, call `init()` to initialize the underlying
     *   `RendererApi`.
     * - Use static methods like `setViewport`, `setClearColor`, `clear`, and `drawIndexed`
     *   for rendering operations.
     *
     * Notes:
     * - The specific implementation of `RendererApi` (e.g., `OpenGlRendererApi`) is
     *   determined by the preprocessor directive `GRAPHICS_API_OPENGL` or similar.
     */
    class RenderCommand {
        public:
            /**
             * @brief Initializes the rendering API.
             *
             * This method initializes the underlying `RendererApi` instance, ensuring that the
             * graphics API is ready to accept rendering commands. It must be called before issuing
             * any other render commands.
             *
             * Throws:
             * - UnknownGraphicsApi exception if the `_rendererApi` instance is null.
             *
             * Usage:
             * - Typically called once during application initialization.
             */
            static void init();

            /**
             * @brief Sets the viewport dimensions and position.
             *
             * The viewport defines the rectangular region of the window where rendering will
             * take place. This command is delegated to the `RendererApi` implementation.
             *
             * @param x The x-coordinate of the lower-left corner of the viewport.
             * @param y The y-coordinate of the lower-left corner of the viewport.
             * @param width The width of the viewport in pixels.
             * @param height The height of the viewport in pixels.
             *
             * Usage:
             * - Call this method whenever the window is resized to adjust the rendering area.
             */
            static void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) { _rendererApi->setViewport(x, y, width, height); };

            /**
            * @brief Sets the clear color for the rendering context.
            *
            * The clear color is used when clearing the color buffer (e.g., during a call to
            * `clear`). This command is delegated to the `RendererApi` implementation.
            *
            * @param color The color to set as the clear color, represented as a `glm::vec4`
            *              (RGBA format with components in the range [0, 1]).
            *
            * Usage:
            * - Typically called once during application initialization or when changing the
            *   background color.
            */
            static void setClearColor(const glm::vec4 &color) { _rendererApi->setClearColor(color); };

            /**
             * @brief Clears the screen using the current clear color.
             *
             * This method clears the color and/or depth buffers, preparing the screen for
             * rendering the next frame. This command is delegated to the `RendererApi`
             * implementation.
             *
             * Usage:
             * - Call this method at the beginning of each frame to reset the screen.
             */
            static void clear() { _rendererApi->clear(); };

            /**
             * @brief Draws indexed primitives using the specified vertex array.
             *
             * This method issues a draw call for rendering indexed geometry. The indices
             * are provided by the index buffer bound to the vertex array. This command is
             * delegated to the `RendererApi` implementation.
             *
             * @param vertexArray A shared pointer to the vertex array containing the geometry data.
             * @param indexCount The number of indices to draw. If set to 0, the method will use
             *                   the total index count from the bound index buffer.
             *
             * Usage:
             * - Use this method to draw meshes or primitives with indexed geometry.
             */
            static void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray, unsigned int indexCount = 0)
            {
                _rendererApi->drawIndexed(vertexArray, indexCount);
            }

        private:
            /**
            * @brief Static pointer to the active `RendererApi` implementation.
            *
            * This member holds a pointer to the concrete `RendererApi` instance (e.g.,
            * `OpenGlRendererApi`). It is initialized based on the active graphics API,
            * as determined by preprocessor directives (e.g., `GRAPHICS_API_OPENGL`).
            *
            * Notes:
            * - The `_rendererApi` instance is statically allocated and shared across all
            *   `RenderCommand` methods.
            * - The application must ensure that `_rendererApi` is initialized via `init()`
            *   before issuing any render commands.
            */
            static RendererApi *_rendererApi;
    };
}
