//// OpenGlRendererAPI.hpp ////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
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
    * @class NxOpenGlRendererApi
    * @brief Implementation of the RendererApi interface using OpenGL.
    *
    * The `NxOpenGlRendererApi` class provides OpenGL-specific implementations for the
    * methods defined in `NxRendererApi`. It interacts directly with OpenGL functions
    * to configure and manage rendering operations.
    *
    * Responsibilities:
    * - Manage OpenGL state for viewport, buffer clearing, and rendering.
    * - Provide access to OpenGL-specific features like blending and depth testing.
    * - Issue draw calls for indexed geometry.
    */
    class NxOpenGlRendererApi final : public NxRendererApi {
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
            * - NxGraphicsApiNotInitialized if OpenGL fails to initialize.
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
            * - NxGraphicsApiViewportResizingFailure if the dimensions exceed the maximum allowed size.
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
            * - NxGraphicsApiNotInitialized if OpenGL is not initialized.
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
            * - NxGraphicsApiNotInitialized if OpenGL is not initialized.
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
            * - NxGraphicsApiNotInitialized if OpenGL is not initialized.
            */
            void setClearDepth(float depth) override;

            void setDepthTest(bool enable) override;
            void setDepthFunc(unsigned int func) override;
            void setDepthMask(bool enable) override;

            /**
             * @brief Renders indexed geometry using OpenGL.
             *
             * Issues a draw call for indexed primitives using data from the specified `NxVertexArray`.
             *
             * @param vertexArray A shared pointer to the `NxVertexArray` containing vertex and index data.
             * @param indexCount The number of indices to draw. If zero, all indices in the buffer are used.
             *
             * Throws:
             * - NxGraphicsApiNotInitialized if OpenGL is not initialized.
             * - NxInvalidValue if the `vertexArray` is null.
             */
            void drawIndexed(const std::shared_ptr<NxVertexArray> &vertexArray, unsigned int indexCount = 0) override;

            void drawUnIndexed(unsigned int verticesCount) override;

            void setStencilTest(bool enable) override;
            void setStencilMask(unsigned int mask) override;
            void setStencilFunc(unsigned int func, int ref, unsigned int mask) override;
            void setStencilOp(unsigned int sfail, unsigned int dpfail, unsigned int dppass) override;

            void setCulling(bool enable) override;
            void setCulledFace(CulledFace face) override;
            void setWindingOrder(WindingOrder order) override;
        private:
            bool m_initialized = false;
            unsigned int m_maxWidth = 0;
            unsigned int m_maxHeight = 0;
    };
}
