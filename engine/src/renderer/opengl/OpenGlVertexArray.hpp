//// OpenGlVertexArray.hpp ////////////////////////////////////////////////////
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
//  Description: Header file for opengl vertex array class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderer/VertexArray.hpp"

namespace nexo::renderer {

    /**
     * @class NxOpenGlVertexArray
     * @brief OpenGL-specific implementation of the `NxVertexArray` class.
     *
     * The `NxOpenGlVertexArray` class manages vertex and index buffers in an OpenGL
     * context. It handles the configuration of vertex attributes and facilitates
     * binding/unbinding of the vertex array for rendering.
     *
     * Responsibilities:
     * - Create and manage an OpenGL vertex array object (VAO).
     * - Configure vertex attributes using buffer layouts.
     * - Bind/unbind the VAO for rendering operations.
     */
    class NxOpenGlVertexArray final : public NxVertexArray {
       public:
        /**
         * @brief Creates an OpenGL vertex array object (VAO).
         *
         * Initializes a new VAO and assigns it a unique ID. This ID is used to reference
         * the VAO in OpenGL operations.
         */
        NxOpenGlVertexArray();
        ~NxOpenGlVertexArray() override = default;

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
         * @throw NxInvalidValue If the vertex buffer is null.
         * @throw NxBufferLayoutEmpty If the vertex buffer's layout is empty.
         */
        void addVertexBuffer(const std::shared_ptr<NxVertexBuffer> &vertexBuffer) override;

        /**
         * @brief Sets the index buffer for the vertex array.
         *
         * Associates an index buffer with the vertex array, enabling indexed rendering.
         *
         * @param indexBuffer The index buffer to set.
         * @throw NxInvalidValue If the index buffer is null.
         */
        void setIndexBuffer(const std::shared_ptr<NxIndexBuffer> &indexBuffer) override;

        /**
         * @brief Retrieves the list of vertex buffers associated with the vertex array.
         *
         * Returns a constant reference to the vector of shared pointers to the vertex
         * buffers that have been added to the vertex array.
         *
         * @return A constant reference to the vector of vertex buffers.
         */
        [[nodiscard]] const std::vector<std::shared_ptr<NxVertexBuffer>> &getVertexBuffers() const override;

        /**
         * @brief Retrieves the index buffer associated with the vertex array.
         *
         * Returns a constant reference to the shared pointer of the index buffer
         * set for the vertex array.
         *
         * @return A constant reference to the index buffer.
         */
        [[nodiscard]] const std::shared_ptr<NxIndexBuffer> &getIndexBuffer() const override;

        /**
         * @brief Retrieves the OpenGL ID of the vertex array object (VAO).
         *
         * Returns the unique identifier assigned to the VAO by OpenGL. This ID is
         * used for binding and managing the VAO within the OpenGL context.
         *
         * @return The OpenGL VAO ID as an unsigned integer.
         */
        [[nodiscard]] unsigned int getId() const override;

       private:
        std::vector<std::shared_ptr<NxVertexBuffer>> _vertexBuffers;
        std::shared_ptr<NxIndexBuffer> _indexBuffer;

        unsigned int _id{};
    };

} // namespace nexo::renderer
