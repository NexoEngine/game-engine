//// RenderCommand.hpp ////////////////////////////////////////////////////////
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
//  Description: Header file for render command class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "RendererAPI.hpp"
#include "DrawCommand.hpp"

namespace nexo::renderer {
    /**
     * @class NxRenderCommand
     * @brief Provides a high-level interface for issuing rendering commands.
     *
     * The `NxRenderCommand` class serves as an abstraction layer over the graphics API
     * (e.g., OpenGL, DirectX, Vulkan), allowing the application to issue rendering
     * commands without being tightly coupled to a specific API.
     *
     * Features:
     * - Initialization and management of the renderer API.
     * - High-level rendering commands such as setting the viewport, clearing the screen,
     *   and drawing indexed primitives.
     *
     * Responsibilities:
     * - Delegates rendering operations to the underlying `NxRendererApi` implementation.
     * - Provides static methods for commonly used rendering commands.
     *
     * Usage:
     * - Before issuing any rendering commands, call `init()` to initialize the underlying
     *   `NxRendererApi`.
     * - Use static methods like `setViewport`, `setClearColor`, `clear`, and `drawIndexed`
     *   for rendering operations.
     *
     * Notes:
     * - The specific implementation of `RendererApi` (e.g., `NxOpenGlRendererApi`) is
     *   determined by the preprocessor directive `NX_GRAPHICS_API_OPENGL` or similar.
     */
    class NxRenderCommand {
       public:
        /**
         * @brief Initializes the rendering API.
         *
         * This method initializes the underlying `NxRendererApi` instance, ensuring that the
         * graphics API is ready to accept rendering commands. It must be called before issuing
         * any other render commands.
         *
         * Throws:
         * - NxUnknownGraphicsApi exception if the `_rendererApi` instance is null.
         *
         * Usage:
         * - Typically called once during application initialization.
         */
        static void init();

        /**
         * @brief Sets the viewport dimensions and position.
         *
         * The viewport defines the rectangular region of the window where rendering will
         * take place. This command is delegated to the `NxRendererApi` implementation.
         *
         * @param x The x-coordinate of the lower-left corner of the viewport.
         * @param y The y-coordinate of the lower-left corner of the viewport.
         * @param width The width of the viewport in pixels.
         * @param height The height of the viewport in pixels.
         *
         * Usage:
         * - Call this method whenever the window is resized to adjust the rendering area.
         */
        static void setViewport(const unsigned int x, const unsigned int y, const unsigned int width,
                                const unsigned int height)
        {
            _rendererApi->setViewport(x, y, width, height);
        }

        /**
         * @brief Sets the clear color for the rendering context.
         *
         * The clear color is used when clearing the color buffer (e.g., during a call to
         * `clear`). This command is delegated to the `NxRendererApi` implementation.
         *
         * @param color The color to set as the clear color, represented as a `glm::vec4`
         *              (RGBA format with components in the range [0, 1]).
         *
         * Usage:
         * - Typically called once during application initialization or when changing the
         *   background color.
         */
        static void setClearColor(const glm::vec4 &color)
        {
            _rendererApi->setClearColor(color);
        }

        /**
         * @brief Sets the clear depth value for the depth buffer.
         *
         * The clear depth value is used when clearing the depth buffer (e.g., during a call to
         * `clear`). This command is delegated to the `NxRendererApi` implementation.
         *
         * @param depth The depth value to set as the clear depth, typically in the range [0, 1].
         *
         * Usage:
         * - Typically called once during application initialization or when changing the
         *   depth clearing behavior.
         */
        static void setClearDepth(const float depth)
        {
            _rendererApi->setClearDepth(depth);
        }

        static void setLineWidth(float lineWidth)
        {
            _rendererApi->setLineWidth(lineWidth);
        }

        /**
         * @brief Clears the screen using the current clear color.
         *
         * This method clears the color and/or depth buffers, preparing the screen for
         * rendering the next frame. This command is delegated to the `NxRendererApi`
         * implementation.
         *
         * Usage:
         * - Call this method at the beginning of each frame to reset the screen.
         */
        static void clear()
        {
            _rendererApi->clear();
        }

        /**
         * @brief Draws indexed primitives using the specified vertex array.
         *
         * This method issues a draw call for rendering indexed geometry. The indices
         * are provided by the index buffer bound to the vertex array. This command is
         * delegated to the `NxRendererApi` implementation.
         *
         * @param vertexArray A shared pointer to the vertex array containing the geometry data.
         * @param indexCount The number of indices to draw. If set to 0, the method will use
         *                   the total index count from the bound index buffer.
         *
         * Usage:
         * - Use this method to draw meshes or primitives with indexed geometry.
         */
        static void drawIndexed(const std::shared_ptr<NxVertexArray> &vertexArray, const size_t indexCount = 0, CommandType primitiveType = CommandType::MESH)
        {
            _rendererApi->drawIndexed(vertexArray, indexCount, primitiveType);
        }

        /**
         * @brief Draws non-indexed primitives.
         *
         * This method issues a draw call for rendering non-indexed geometry. It uses
         * the vertex data directly from the bound vertex buffer. This command is
         * delegated to the `NxRendererApi` implementation.
         *
         * @param verticesCount The number of vertices to draw.
         *
         * Usage:
         * - Use this method to draw simple primitives or geometry without an index buffer.
         */
        static void drawUnIndexed(const size_t verticesCount)
        {
            _rendererApi->drawUnIndexed(verticesCount);
        }

        /**
         * @brief Enables or disables the depth test.
         *
         * The depth test determines whether a fragment should be drawn based on its depth
         * value compared to the existing value in the depth buffer. When enabled, fragments
         * that are behind existing geometry will be discarded.
         *
         * @param enable True to enable depth testing, false to disable it.
         *
         * Usage:
         * - Enable the depth test when rendering 3D scenes to ensure correct occlusion.
         * - Disable the depth test for 2D rendering or when rendering transparent objects.
         */
        static void setDepthTest(const bool enable)
        {
            _rendererApi->setDepthTest(enable);
        }

        /**
         * @brief Enables or disables writing to the depth buffer.
         *
         * When depth writing is enabled, the depth values of rendered fragments will
         * be written to the depth buffer. Disabling depth writing can be useful for
         * rendering transparent objects or overlays where depth information should not
         * be updated.
         *
         * @param enable True to enable depth writing, false to disable it.
         *
         * Usage:
         * - Enable depth writing when rendering opaque geometry.
         * - Disable depth writing when rendering transparent objects or UI elements.
         */
        static void setDepthMask(const bool enable)
        {
            _rendererApi->setDepthMask(enable);
        }

        /**
         * @brief Sets the depth comparison function used for depth testing.
         *
         * The depth function determines how the depth of incoming fragments is compared
         * to the existing depth values in the depth buffer. Common functions include
         * GL_LESS, GL_LEQUAL, GL_GREATER, etc.
         *
         * @param func The depth comparison function to use (e.g., GL_LESS, GL_ALWAYS).
         *
         * Usage:
         * - Set the depth function based on the desired depth testing behavior for your scene.
         */
        static void setDepthFunc(const unsigned int func)
        {
            _rendererApi->setDepthFunc(func);
        }

        /**
         * @brief Enables or disables the stencil test.
         *
         * The stencil test allows for masking certain portions of the screen during rendering.
         * When enabled, fragments are drawn only if they pass a comparison test against the
         * corresponding value in the stencil buffer.
         *
         * @param enable True to enable stencil testing, false to disable it.
         *
         * Usage:
         * - Enable the stencil test before performing operations that will write to or use the stencil buffer.
         * - Disable the stencil test when regular rendering should resume.
         */
        static void setStencilTest(const bool enable)
        {
            _rendererApi->setStencilTest(enable);
        }

        /**
         * @brief Sets the stencil mask that controls which bits of the stencil buffer are updated.
         *
         * The stencil mask determines which bits in the stencil buffer can be modified when
         * stencil operations are performed. Only the bits that have a 1 in the corresponding
         * position of the mask will be affected.
         *
         * @param mask The bit mask to use for stencil write operations.
         *
         * Usage:
         * - Set a specific mask before performing stencil operations to control which bits are affected.
         */
        static void setStencilMask(const unsigned int mask)
        {
            _rendererApi->setStencilMask(mask);
        }

        /**
         * @brief Configures the stencil function used for stencil testing.
         *
         * The stencil function defines how the stencil test compares a reference value to the
         * current value in the stencil buffer. The comparison result determines whether a fragment
         * passes the stencil test and how the stencil buffer is updated.
         *
         * @param func The comparison function to use (e.g., GL_EQUAL, GL_ALWAYS, GL_LESS).
         * @param ref The reference value to compare against.
         * @param mask The mask that is ANDed with both the reference value and stored stencil value before comparison.
         *
         * Usage:
         * - Configure before performing operations that rely on specific stencil buffer values.
         */
        static void setStencilFunc(const unsigned int func, const int ref, const unsigned int mask)
        {
            _rendererApi->setStencilFunc(func, ref, mask);
        }

        /**
         * @brief Sets the operations to perform on the stencil buffer based on test outcomes.
         *
         * This method configures what happens to the stencil buffer value when the stencil test:
         * - fails (sfail)
         * - passes, but the depth test fails (dpfail)
         * - passes, and the depth test also passes (dppass)
         *
         * @param sfail Operation to perform when the stencil test fails.
         * @param dpfail Operation to perform when the stencil test passes but depth test fails.
         * @param dppass Operation to perform when both stencil and depth tests pass.
         *
         * Usage:
         * - Set before performing complex stencil operations like object outlining or shadow volumes.
         */
        static void setStencilOp(const unsigned int sfail, const unsigned int dpfail, const unsigned int dppass)
        {
            _rendererApi->setStencilOp(sfail, dpfail, dppass);
        }

        /**
         * @brief Enables or disables face culling.
         *
         * Face culling is a performance optimization technique that prevents the rendering
         * of faces (triangles) that are not visible to the camera. When enabled, back faces
         * (or front faces, depending on the winding order) are not drawn.
         *
         * @param enable True to enable face culling, false to disable it.
         *
         * Usage:
         * - Enable face culling when rendering closed 3D models to improve performance.
         * - Disable face culling for transparent objects or when both sides of a surface need to be visible.
         */
        static void setCulling(const bool enable)
        {
            _rendererApi->setCulling(enable);
        }

        /**
         * @brief Specifies which face (front or back) should be culled.
         *
         * This method sets whether front faces or back faces of polygons should be culled
         * when face culling is enabled. The choice depends on the winding order of the vertices.
         *
         * @param face The face to cull (e.g., BACK_FACE, FRONT_FACE).
         *
         * Usage:
         * - Set according to the winding order of your models and the desired culling behavior.
         */
        static void setCulledFace(const CulledFace face)
        {
            _rendererApi->setCulledFace(face);
        }

        /**
         * @brief Sets the winding order used to determine front-facing polygons.
         *
         * The winding order defines the order in which vertices are specified for a polygon
         * (clockwise or counter-clockwise). This setting is used to determine which faces
         * are considered front-facing and which are back-facing for culling purposes.
         *
         * @param order The winding order to use (e.g., CLOCKWISE, COUNTER_CLOCKWISE).
         *
         * Usage:
         * - Set according to the vertex order used in your models to ensure correct face culling.
         */
        static void setWindingOrder(const WindingOrder order)
        {
            _rendererApi->setWindingOrder(order);
        }

       private:
        /**
         * @brief Static pointer to the active `NxRendererApi` implementation.
         *
         * This member holds a pointer to the concrete `NxRendererApi` instance (e.g.,
         * `NxOpenGlRendererApi`). It is initialized based on the active graphics API,
         * as determined by preprocessor directives (e.g., `NX_GRAPHICS_API_OPENGL`).
         *
         * Notes:
         * - The `_rendererApi` instance is statically allocated and shared across all
         *   `NxRenderCommand` methods.
         * - The application must ensure that `_rendererApi` is initialized via `init()`
         *   before issuing any render commands.
         */
        static NxRendererApi *_rendererApi;
    };
} // namespace nexo::renderer
