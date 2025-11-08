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

        void setLineWidth(float lineWidth) override;

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
        void drawIndexed(const std::shared_ptr<NxVertexArray> &vertexArray, size_t indexCount = 0, CommandType primitiveType = CommandType::MESH) override;

        void drawIndexedInstanced(const std::shared_ptr<NxVertexArray> &vertexArray, size_t indexCount = 0, unsigned int instanceCount = 1, CommandType primitiveType = CommandType::MESH) override;

        /**
         * @brief Renders non-indexed geometry using OpenGL.
         *
         * Issues a draw call for non-indexed primitives using the specified number of vertices.
         *
         * @param verticesCount The number of vertices to draw.
         *
         * Throws:
         * - NxGraphicsApiNotInitialized if OpenGL is not initialized.
         */
        void drawUnIndexed(size_t verticesCount) override;

        /**
         * @brief Enables or disables stencil testing in OpenGL.
         *
         * Configures whether stencil testing is active during rendering operations.
         *
         * @param enable A boolean indicating whether to enable (true) or disable (false) stencil testing.
         *
         * Throws:
         * - NxGraphicsApiNotInitialized if OpenGL is not initialized.
         */
        void setStencilTest(bool enable) override;

        /**
         * @brief Sets the stencil mask for OpenGL operations.
         *
         * Configures the stencil mask, which determines which bits of the stencil buffer
         * can be written during stencil operations.
         *
         * @param mask The bitmask to apply to stencil buffer writes.
         *
         * Throws:
         * - NxGraphicsApiNotInitialized if OpenGL is not initialized.
         */
        void setStencilMask(unsigned int mask) override;

        /**
         * @brief Configures the stencil test function in OpenGL.
         *
         * Sets the stencil test function, reference value, and mask used to compare
         * stencil buffer values during rendering.
         *
         * @param func The stencil test function (e.g., GL_ALWAYS, GL_NEVER, GL_EQUAL).
         * @param ref The reference value for the stencil test.
         * @param mask The bitmask applied to the reference and stencil buffer values.
         *
         * Throws:
         * - NxGraphicsApiNotInitialized if OpenGL is not initialized.
         */
        void setStencilFunc(unsigned int func, int ref, unsigned int mask) override;

        /**
         * @brief Configures the stencil operations for OpenGL.
         *
         * Sets the actions to take when stencil tests fail, depth tests fail, or both pass.
         *
         * @param sfail Action to take when the stencil test fails.
         * @param dpfail Action to take when the stencil test passes but the depth test fails.
         * @param dppass Action to take when both the stencil and depth tests pass.
         *
         * Throws:
         * - NxGraphicsApiNotInitialized if OpenGL is not initialized.
         */
        void setStencilOp(unsigned int sfail, unsigned int dpfail, unsigned int dppass) override;

        /**
         * @brief Enables or disables face culling in OpenGL.
         *
         * Configures whether face culling is active during rendering operations.
         *
         * @param enable A boolean indicating whether to enable (true) or disable (false) face culling.
         *
         * Throws:
         * - NxGraphicsApiNotInitialized if OpenGL is not initialized.
         */
        void setCulling(bool enable) override;

        /**
         * @brief Sets the face to be culled in OpenGL.
         *
         * Configures whether front, back, or both faces are culled during rendering.
         *
         * @param face The face to cull (e.g., CulledFace::FRONT, CulledFace::BACK).
         *
         * Throws:
         * - NxGraphicsApiNotInitialized if OpenGL is not initialized.
         */
        void setCulledFace(CulledFace face) override;

        /**
         * @brief Sets the winding order for face culling in OpenGL.
         *
         * Configures the vertex winding order (clockwise or counterclockwise) used
         * to determine front-facing polygons.
         *
         * @param order The winding order (e.g., WindingOrder::CW, WindingOrder::CCW).
         *
         * Throws:
         * - NxGraphicsApiNotInitialized if OpenGL is not initialized.
         */
        void setWindingOrder(WindingOrder order) override;

       private:
        bool m_initialized       = false;
        unsigned int m_maxWidth  = 0;
        unsigned int m_maxHeight = 0;
    };
} // namespace nexo::renderer
