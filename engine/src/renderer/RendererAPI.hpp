//// RendererAPI.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for renderer api interface
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "VertexArray.hpp"
#include "DrawCommand.hpp"

namespace nexo::renderer {

    enum class CulledFace { BACK, FRONT, FRONT_AND_BACK };

    enum class WindingOrder { CW, CCW };

    /**
     * @class NxRendererApi
     * @brief Abstract interface for low-level rendering API implementations.
     *
     * The `NxRendererApi` class defines the essential methods required for interacting
     * with the graphics pipeline, such as initializing the API, configuring the
     * viewport, clearing buffers, and issuing draw commands. Specific graphics APIs,
     * like OpenGL, DirectX, or Vulkan, should implement this interface to ensure
     * compatibility with the renderer framework.
     *
     * Responsibilities:
     * - Provide a uniform interface for graphics API interaction.
     * - Define methods for initializing and managing rendering settings.
     * - Support commands for clearing buffers, setting viewport size, and drawing.
     *
     * Subclasses:
     * - `NxOpenGlRendererApi`: Implements this interface using OpenGL commands.
     */
    class NxRendererApi {
       public:
        virtual ~NxRendererApi() = default;

        /**
         * @brief Initializes the graphics API.
         *
         * This method sets up the necessary states and configurations required for
         * rendering, such as enabling blending, depth testing, and face culling.
         *
         * Must be implemented by subclasses.
         *
         * Throws:
         * - Exceptions specific to the API if initialization fails.
         */
        virtual void init() = 0;

        /**
         * @brief Sets the dimensions and position of the viewport.
         *
         * Configures the viewport area where rendering will occur. The width and height
         * define the size of the viewport, and x and y specify its position.
         *
         * @param x The x-coordinate of the viewport's bottom-left corner.
         * @param y The y-coordinate of the viewport's bottom-left corner.
         * @param width The width of the viewport in pixels.
         * @param height The height of the viewport in pixels.
         *
         * Must be implemented by subclasses.
         */
        virtual void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;

        /**
         * @brief Retrieves the maximum dimensions supported by the graphics API for the viewport.
         *
         * @param[out] width Pointer to store the maximum viewport width.
         * @param[out] height Pointer to store the maximum viewport height.
         *
         * Must be implemented by subclasses.
         */
        virtual void getMaxViewportSize(unsigned int *width, unsigned int *height) = 0;

        /**
         * @brief Clears the color and depth buffers.
         *
         * Resets the frame buffer by clearing the current color and depth data.
         *
         * Must be implemented by subclasses.
         */
        virtual void clear() = 0;

        /**
         * @brief Sets the color used to clear the frame buffer.
         *
         * Configures the RGBA color that will fill the frame buffer during clearing.
         *
         * @param color A `glm::vec4` containing the red, green, blue, and alpha components of the clear color.
         *
         * Must be implemented by subclasses.
         */
        virtual void setClearColor(const glm::vec4 &color) = 0;

        /**
         * @brief Sets the depth value used to clear the depth buffer.
         *
         * Configures the depth value that will fill the depth buffer during clearing.
         *
         * @param depth A float value representing the clear depth.
         *
         * Must be implemented by subclasses.
         */
        virtual void setClearDepth(float depth) = 0;

        /**
         * @brief Controls the depth testing operation.
         *
         * Enables or disables depth testing during rendering operations.
         *
         * @param enable True to enable depth testing, false to disable it.
         *
         * Must be implemented by subclasses.
         */
        virtual void setDepthTest(bool enable) = 0;

        /**
         * @brief Sets the depth comparison function.
         *
         * Configures how the depth values of incoming fragments are compared with
         * the values stored in the depth buffer.
         *
         * @param func The depth comparison function to use (e.g., LESS, GREATER, EQUAL).
         *
         * Must be implemented by subclasses.
         */
        virtual void setDepthFunc(unsigned int func) = 0;

        /**
         * @brief Controls writing to the depth buffer.
         *
         * Enables or disables writing depth values to the depth buffer during rendering.
         *
         * @param enable True to enable depth writes, false to disable them.
         *
         * Must be implemented by subclasses.
         */
        virtual void setDepthMask(bool enable) = 0;

        virtual void setLineWidth(float lineWidth) = 0;

        /**
         * @brief Issues a draw call for indexed geometry.
         *
         * Renders geometry using indices stored in the index buffer attached to the
         * specified `NxVertexArray`.
         *
         * @param vertexArray A shared pointer to the `NxVertexArray` containing vertex and index data.
         * @param count The number of indices to draw. If zero, all indices in the buffer are used.
         *
         * Must be implemented by subclasses.
         */
        virtual void drawIndexed(const std::shared_ptr<NxVertexArray> &vertexArray, size_t count = 0, CommandType primitiveType = CommandType::MESH) = 0;

        /**
         * @brief Draws geometry without using indices.
         *
         * Renders vertices directly from the vertex buffer in sequential order.
         *
         * @param verticesCount The number of vertices to draw.
         *
         * Must be implemented by subclasses.
         */
        virtual void drawUnIndexed(size_t verticesCount) = 0;

        /**
         * @brief Controls the stencil testing operation.
         *
         * @param enable True to enable stencil testing, false to disable it.
         *
         * Must be implemented by subclasses.
         */
        virtual void setStencilTest(bool enable) = 0;

        /**
         * @brief Sets the mask for writing to the stencil buffer.
         *
         * @param mask The mask that controls which bits are written to the stencil buffer.
         *
         * Must be implemented by subclasses.
         */
        virtual void setStencilMask(unsigned int mask) = 0;

        /**
         * @brief Configures the stencil test function.
         *
         * @param func The stencil comparison function to use.
         * @param ref The reference value for stencil testing.
         * @param mask The mask that is ANDed with both the reference value and stored stencil value.
         *
         * Must be implemented by subclasses.
         */
        virtual void setStencilFunc(unsigned int func, int ref, unsigned int mask) = 0;

        /**
         * @brief Sets the stencil buffer operations.
         *
         * @param sfail Operation to perform when stencil test fails.
         * @param dpfail Operation when stencil test passes but depth test fails.
         * @param dppass Operation when both stencil and depth tests pass.
         *
         * Must be implemented by subclasses.
         */
        virtual void setStencilOp(unsigned int sfail, unsigned int dpfail, unsigned int dppass) = 0;

        /**
         * @brief Enables or disables face culling.
         *
         * @param enable True to enable face culling, false to disable it.
         *
         * Must be implemented by subclasses.
         */
        virtual void setCulling(bool enable) = 0;

        /**
         * @brief Sets which faces should be culled.
         *
         * @param face The face(s) to be culled (BACK, FRONT, or FRONT_AND_BACK).
         *
         * Must be implemented by subclasses.
         */
        virtual void setCulledFace(CulledFace face) = 0;

        /**
         * @brief Sets the winding order for front face determination.
         *
         * @param order The winding order (CW for clockwise, CCW for counter-clockwise).
         *
         * Must be implemented by subclasses.
         */
        virtual void setWindingOrder(WindingOrder order) = 0;
    };

} // namespace nexo::renderer
