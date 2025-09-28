//// VertexArray.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for vertex array abstract class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Buffer.hpp"

namespace nexo::renderer {

    /**
     * @class NxVertexArray
     * @brief Abstract class representing a vertex array in the rendering system.
     *
     * The `NxVertexArray` class manages the collection of vertex buffers and an optional
     * index buffer. It provides the interface for binding, unbinding, and configuring
     * vertex attributes in the rendering pipeline.
     *
     * Responsibilities:
     * - Manage vertex buffers and index buffers.
     * - Bind/unbind the vertex array for rendering.
     * - Provide access to underlying buffers.
     *
     * Derived classes (e.g., `NxOpenGlVertexArray`) implement platform-specific behavior
     * for managing vertex arrays.
     */
    class NxVertexArray {
       public:
        virtual ~NxVertexArray() = default;

        /**
         * @brief Binds the vertex array, making it active for rendering.
         *
         * This method binds the vertex array object (VAO) to the current graphics context,
         * allowing subsequent draw calls to use the vertex and index buffers associated
         * with this vertex array.
         *
         * Throws:
         * - Implementation-specific exceptions if binding fails.
         *
         * Notes:
         * - The actual implementation of this method is platform-specific and should be
         *   provided by derived classes.
         */
        virtual void bind() const = 0;

        /**
         * @brief Unbinds the vertex array, detaching it from the current context.
         *
         * This method unbinds the vertex array object (VAO) from the current graphics context,
         * effectively disabling its use for subsequent draw calls. This is useful for ensuring
         * that no unintended modifications are made to the vertex array state.
         *
         * Notes:
         * - The actual implementation of this method is platform-specific and should be
         *   provided by derived classes.
         */
        virtual void unbind() const = 0;

        /**
         * @brief Adds a vertex buffer to the vertex array.
         *
         * This method associates a vertex buffer with the vertex array, allowing it to be
         * used for rendering. The vertex buffer contains vertex attribute data such as
         * positions, normals, texture coordinates, etc.
         *
         * @param vertexBuffer A shared pointer to the `NxVertexBuffer` to be added.
         *
         * Throws:
         * - Implementation-specific exceptions if adding the vertex buffer fails.
         *
         * Notes:
         * - The actual implementation of this method is platform-specific and should be
         *   provided by derived classes.
         */
        virtual void addVertexBuffer(const std::shared_ptr<NxVertexBuffer> &vertexBuffer) = 0;

        /**
         * @brief Sets the index buffer for the vertex array.
         *
         * This method associates an index buffer with the vertex array, allowing indexed
         * drawing operations. The index buffer contains indices that reference vertices
         * in the vertex buffers, enabling efficient reuse of vertex data.
         *
         * @param indexBuffer A shared pointer to the `NxIndexBuffer` to be set.
         *
         * Throws:
         * - Implementation-specific exceptions if setting the index buffer fails.
         *
         * Notes:
         * - The actual implementation of this method is platform-specific and should be
         *   provided by derived classes.
         */
        virtual void setIndexBuffer(const std::shared_ptr<NxIndexBuffer> &indexBuffer) = 0;

        /**
         * @brief Retrieves the list of vertex buffers associated with the vertex array.
         *
         * This method returns a constant reference to a vector containing shared pointers
         * to all vertex buffers that have been added to the vertex array. This allows
         * inspection of the vertex buffers for debugging or configuration purposes.
         *
         * @return A constant reference to a vector of shared pointers to `NxVertexBuffer` instances.
         */
        [[nodiscard]] virtual const std::vector<std::shared_ptr<NxVertexBuffer>> &getVertexBuffers() const = 0;

        /**
         * @brief Retrieves the index buffer associated with the vertex array.
         *
         * This method returns a constant reference to the shared pointer of the index buffer
         * that has been set for the vertex array. If no index buffer has been set, it may
         * return a null pointer.
         *
         * @return A constant reference to the shared pointer of the `NxIndexBuffer` instance.
         */
        [[nodiscard]] virtual const std::shared_ptr<NxIndexBuffer> &getIndexBuffer() const = 0;

        /**
         * @brief Retrieves the unique identifier of the vertex array.
         *
         * This method returns an unsigned integer that uniquely identifies the vertex array
         * within the graphics context. This ID is typically used for debugging or low-level
         * graphics operations.
         *
         * @return The unique ID of the vertex array.
         */
        [[nodiscard]] virtual unsigned int getId() const = 0;
    };

    /**
     * @brief Factory function to create a platform-specific vertex array object.
     *
     * Depending on the graphics API (e.g., OpenGL), this function creates an instance
     * of the corresponding `NxVertexArray` implementation.
     *
     * @return A shared pointer to the created `NxVertexArray` instance.
     */
    std::shared_ptr<NxVertexArray> createVertexArray();
} // namespace nexo::renderer
