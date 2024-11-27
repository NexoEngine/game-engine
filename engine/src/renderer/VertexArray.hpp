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
    * @class VertexArray
    * @brief Abstract class representing a vertex array in the rendering system.
    *
    * The `VertexArray` class manages the collection of vertex buffers and an optional
    * index buffer. It provides the interface for binding, unbinding, and configuring
    * vertex attributes in the rendering pipeline.
    *
    * Responsibilities:
    * - Manage vertex buffers and index buffers.
    * - Bind/unbind the vertex array for rendering.
    * - Provide access to underlying buffers.
    *
    * Derived classes (e.g., `OpenGlVertexArray`) implement platform-specific behavior
    * for managing vertex arrays.
    */
    class VertexArray {
        public:
            virtual ~VertexArray() = default;

            virtual void bind() const = 0;
            virtual void unbind() const = 0;

            virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) = 0;
            virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) = 0;

            [[nodiscard]] virtual const std::vector<std::shared_ptr<VertexBuffer>> &getVertexBuffers() const = 0;
            [[nodiscard]] virtual const std::shared_ptr<IndexBuffer> &getIndexBuffer() const = 0;

            virtual unsigned int getId() const = 0;
    };

    /**
    * @brief Factory function to create a platform-specific vertex array object.
    *
    * Depending on the graphics API (e.g., OpenGL), this function creates an instance
    * of the corresponding `VertexArray` implementation.
    *
    * @return A shared pointer to the created `VertexArray` instance.
    */
    std::shared_ptr<VertexArray> createVertexArray();
}
