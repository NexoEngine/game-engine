//// OpenGlVertexArray.hpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for opengl vertex array class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderer/VertexArray.hpp"

namespace nexo::renderer {

    /**
    * @class OpenGlVertexArray
    * @brief OpenGL-specific implementation of the `VertexArray` class.
    *
    * The `OpenGlVertexArray` class manages vertex and index buffers in an OpenGL
    * context. It handles the configuration of vertex attributes and facilitates
    * binding/unbinding of the vertex array for rendering.
    *
    * Responsibilities:
    * - Create and manage an OpenGL vertex array object (VAO).
    * - Configure vertex attributes using buffer layouts.
    * - Bind/unbind the VAO for rendering operations.
    */
    class OpenGlVertexArray final : public VertexArray {
        public:
            /**
            * @brief Creates an OpenGL vertex array object (VAO).
            *
            * Initializes a new VAO and assigns it a unique ID. This ID is used to reference
            * the VAO in OpenGL operations.
            */
            OpenGlVertexArray();
            ~OpenGlVertexArray() override = default;

            /**
            * @brief Binds the vertex array object (VAO) to the OpenGL context.
            *
            * Activates the VAO for subsequent rendering operations. This ensures that
            * the vertex and index buffers associated with the VAO are used.
            */
            void bind() const override;

            /**
            * @brief Unbinds the vertex array object (VAO) from the OpenGL context.
            *
            * Deactivates the currently bound VAO. This prevents unintended modifications
            * to the VAO in subsequent OpenGL calls.
            */
            void unbind() const override;

            /**
            * @brief Adds a vertex buffer to the vertex array.
            *
            * Configures the vertex attributes for the given vertex buffer based on its
            * buffer layout. The attributes are assigned sequential indices.
            *
            * @param vertexBuffer The vertex buffer to add.
            * @throw InvalidValue If the vertex buffer is null.
            * @throw BufferLayoutEmpty If the vertex buffer's layout is empty.
            */
            void addVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) override;

            /**
            * @brief Sets the index buffer for the vertex array.
            *
            * Associates an index buffer with the vertex array, enabling indexed rendering.
            *
            * @param indexBuffer The index buffer to set.
            * @throw InvalidValue If the index buffer is null.
            */
            void setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) override;

            [[nodiscard]] const std::vector<std::shared_ptr<VertexBuffer>> &getVertexBuffers() const override;
            [[nodiscard]] const std::shared_ptr<IndexBuffer> &getIndexBuffer() const override;

            unsigned int getId() const override;
        private:
            std::vector<std::shared_ptr<VertexBuffer>> _vertexBuffers;
            std::shared_ptr<IndexBuffer> _indexBuffer;

            unsigned int _id{};
    };

}
