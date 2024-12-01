//// OpenGlRendererAPI.hpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for open gl renderer api class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "renderer/RendererAPI.hpp"

namespace nexo::renderer {

    /**
    * @class OpenGlRendererApi
    * @brief Implementation of the RendererApi interface using OpenGL.
    *
    * The `OpenGlRendererApi` class provides OpenGL-specific implementations for the
    * methods defined in `RendererApi`. It interacts directly with OpenGL functions
    * to configure and manage rendering operations.
    *
    * Responsibilities:
    * - Manage OpenGL state for viewport, buffer clearing, and rendering.
    * - Provide access to OpenGL-specific features like blending and depth testing.
    * - Issue draw calls for indexed geometry.
    */
    class OpenGlRendererApi final : public RendererApi {
        public:
            /**
            * @brief Initializes the OpenGL renderer API.
            *
            * Sets up OpenGL state, including:
            * - Enabling blending for transparent objects.
            * - Enabling depth testing for correct object ordering.
            * - Configuring maximum viewport dimensions.
            *
            * Throws:
            * - GraphicsApiNotInitialized if OpenGL fails to initialize.
            */
            void init() override;

            /**
            * @brief Configures the OpenGL viewport.
            *
            * Sets the OpenGL viewport dimensions and position, ensuring that rendering
            * occurs within the specified bounds.
            *
            * @param x The x-coordinate of the viewport's bottom-left corner.
            * @param y The y-coordinate of the viewport's bottom-left corner.
            * @param width The width of the viewport in pixels.
            * @param height The height of the viewport in pixels.
            *
            * Throws:
            * - GraphicsApiViewportResizingFailure if the dimensions exceed the maximum allowed size.
            */
            void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;

            /**
            * @brief Retrieves the maximum viewport dimensions supported by OpenGL.
            *
            * Queries OpenGL for the maximum allowed dimensions of the viewport and
            * stores the results in the provided pointers.
            *
            * @param[out] width Pointer to store the maximum viewport width.
            * @param[out] height Pointer to store the maximum viewport height.
            */
            void getMaxViewportSize(unsigned int *width, unsigned int *height) override;

            /**
            * @brief Clears the OpenGL frame buffer.
            *
            * Resets the color and depth buffers using the current clear color and depth values.
            *
            * Throws:
            * - GraphicsApiNotInitialized if OpenGL is not initialized.
            */
            void clear() override;

            /**
            * @brief Sets the OpenGL clear color.
            *
            * Configures the RGBA color used to clear the frame buffer during the next call to `clear()`.
            *
            * @param color A `glm::vec4` containing the red, green, blue, and alpha components of the clear color.
            *
            * Throws:
            * - GraphicsApiNotInitialized if OpenGL is not initialized.
            */
            void setClearColor(const glm::vec4 &color) override;

            /**
            * @brief Sets the OpenGL clear depth value.
            *
            * Configures the depth value used to clear the depth buffer during the next call to `clear()`.
            *
            * @param depth A float value representing the clear depth.
            *
            * Throws:
            * - GraphicsApiNotInitialized if OpenGL is not initialized.
            */
            void setClearDepth(float depth) override;

            /**
             * @brief Renders indexed geometry using OpenGL.
             *
             * Issues a draw call for indexed primitives using data from the specified `VertexArray`.
             *
             * @param vertexArray A shared pointer to the `VertexArray` containing vertex and index data.
             * @param indexCount The number of indices to draw. If zero, all indices in the buffer are used.
             *
             * Throws:
             * - GraphicsApiNotInitialized if OpenGL is not initialized.
             * - InvalidValue if the `vertexArray` is null.
             */
            void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray, unsigned int indexCount = 0) override;
        private:
            bool m_initialized = false;
            int m_maxWidth = 0;
            int m_maxHeight = 0;
    };
}
