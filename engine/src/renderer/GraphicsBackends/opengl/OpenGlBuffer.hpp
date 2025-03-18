//// OpenGlBuffer.hpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the opengl buffers (vertex and index)
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderer/Buffer.hpp"

namespace nexo::renderer {

    class OpenGlVertexBuffer final : public VertexBuffer {
        public:
            /**
             * @brief Constructs a new vertex buffer and initializes it with vertex data.
             *
             * This constructor creates a new OpenGL vertex buffer, allocates GPU memory, and
             * uploads the provided vertex data to the buffer.
             *
             * @param vertices Pointer to the array of vertex data to initialize the buffer with.
             * @param size The size (in bytes) of the vertex data to upload.
             *
             * OpenGL Calls:
             * - `glGenBuffers`: Generates a new buffer object.
             * - `glBindBuffer`: Binds the buffer as the current vertex buffer (GL_ARRAY_BUFFER).
             * - `glBufferData`: Allocates GPU memory and uploads the vertex data.
             */
            OpenGlVertexBuffer(const float *vertices, unsigned int size);

            /**
             * @brief Constructs an empty vertex buffer with a specified size.
             *
             * This constructor creates a new OpenGL vertex buffer and allocates GPU memory
             * without uploading any data. The buffer is intended for dynamic updates.
             *
             * @param size The size (in bytes) of the buffer to allocate.
             *
             * OpenGL Calls:
             * - `glGenBuffers`: Generates a new buffer object.
             * - `glBindBuffer`: Binds the buffer as the current vertex buffer (GL_ARRAY_BUFFER).
             * - `glBufferData`: Allocates GPU memory with a `nullptr` for data.
             *
             * Usage:
             * - Call `setData` later to populate the buffer with vertex data dynamically.
             */
            explicit OpenGlVertexBuffer(unsigned int size);

            /**
            * @brief Destroys the vertex buffer and releases GPU resources.
            *
            * This destructor ensures that the OpenGL buffer is deleted, freeing GPU memory.
            *
            * OpenGL Calls:
            * - `glDeleteBuffers`: Deletes the buffer object associated with the buffer ID.
            */
            ~OpenGlVertexBuffer() override;

            /**
             * @brief Binds the vertex buffer as the active buffer in the OpenGL context.
             *
             * OpenGL Calls:
             * - `glBindBuffer`: Binds the buffer as the current vertex buffer (GL_ARRAY_BUFFER).
             *
             * Usage:
             * - Binding the vertex buffer ensures that subsequent OpenGL calls will operate
             *   on this buffer.
             */
            void bind() const override;

            /**
             * @brief Unbinds the current vertex buffer from the OpenGL context.
             *
             * OpenGL Calls:
             * - `glBindBuffer`: Unbinds any currently bound vertex buffer by setting the
             *   active buffer to 0.
             *
             * Usage:
             * - This is optional but ensures no unintended operations are performed on the buffer.
             */
            void unbind() const override;

            void setLayout(const BufferLayout &layout) override { _layout = layout; };
            [[nodiscard]] BufferLayout getLayout() const override { return _layout; };

            /**
             * @brief Updates the data in the vertex buffer.
             *
             * This method replaces the content of the vertex buffer with new data. The data
             * must fit within the allocated buffer size.
             *
             * @param data Pointer to the new vertex data to upload.
             * @param size The size (in bytes) of the data to upload.
             *
             * OpenGL Calls:
             * - `glBindBuffer`: Binds the buffer as the current vertex buffer (GL_ARRAY_BUFFER).
             * - `glBufferSubData`: Updates a subset of the buffer's data.
             *
             * Usage:
             * - Use this method for dynamically updating buffer content.
             */
            void setData(void *data, unsigned int size) override;

            [[nodiscard]] unsigned int getId() const override { return _id; };

        private:
            unsigned int _id{};
            BufferLayout _layout;
    };

    class OpenGlIndexBuffer final : public IndexBuffer {
        public:
            /**
             * @brief Constructs a new OpenGL index buffer.
             *
             * This constructor creates a new index buffer and allocates GPU memory. The
             * buffer is initially empty and must be populated with index data using `setData`.
             *
             * OpenGL Calls:
             * - `glGenBuffers`: Generates a new buffer object.
             * - `glBindBuffer`: Binds the buffer as the current index buffer (GL_ELEMENT_ARRAY_BUFFER).
            */
            OpenGlIndexBuffer();

            /**
             * @brief Destroys the index buffer and releases GPU resources.
             *
             * This destructor ensures that the OpenGL buffer is deleted, freeing GPU memory.
             *
             * OpenGL Calls:
             * - `glDeleteBuffers`: Deletes the buffer object associated with the buffer ID.
            */
            ~OpenGlIndexBuffer() override;

            /**
             * @brief Binds the index buffer as the active buffer in the OpenGL context.
             *
             * OpenGL Calls:
             * - `glBindBuffer`: Binds the buffer as the current index buffer (GL_ELEMENT_ARRAY_BUFFER).
             *
             * Usage:
             * - Binding the index buffer ensures that subsequent OpenGL draw calls will use
             *   this buffer for rendering indexed primitives.
            */
            void bind() const override;
             /**
             * @brief Unbinds the current index buffer from the OpenGL context.
             *
             * OpenGL Calls:
             * - `glBindBuffer`: Unbinds any currently bound index buffer by setting the
             *   active buffer to 0.
             *
             * Usage:
             * - This is optional but ensures no unintended operations are performed on the buffer.
             */
            void unbind() const override;

            /**
            * @brief Uploads index data to the index buffer.
            *
            * This method populates the buffer with an array of indices, which are used
            * for rendering indexed primitives. The buffer must have sufficient memory
            * allocated to store the indices.
            *
            * @param indices Pointer to the array of indices to upload.
            * @param count The number of indices to upload.
            *
            * OpenGL Calls:
            * - `glBufferData`: Allocates GPU memory and uploads the index data.
            *
            * Notes:
            * - Sets the `_count` member to track the number of indices in the buffer.
            */
            void setData(unsigned int *indices, unsigned int count) override;

            /**
            * @brief Retrieves the number of indices in the buffer.
            *
            * This method returns the number of indices currently stored in the index buffer.
            *
            * @return The count of indices in the buffer.
            *
            * Usage:
            * - Use this value for rendering indexed primitives.
            */
            [[nodiscard]] unsigned int getCount() const override;

            /**
 * @brief Retrieves the unique OpenGL buffer ID.
 *
 * Returns the identifier assigned to this buffer object, which is essential for managing
 * and binding the buffer in OpenGL operations.
 *
 * @return The OpenGL buffer ID.
 */
[[nodiscard]] unsigned int getId() const override { return _id; };
        private:
            unsigned int _id{};
            unsigned int _count = 0;
    };

}
