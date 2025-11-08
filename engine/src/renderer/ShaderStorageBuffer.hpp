//// ShaderStorageBuffer.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/02/2025
//  Description: Header file for the SSBOs buffer
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

namespace nexo::renderer {
    class NxShaderStorageBuffer {
       public:
        virtual ~NxShaderStorageBuffer() = default;

        /**
         * @brief Creates a shader storage buffer of the specified size.
         * @param size Size of the buffer in bytes.
         * @return A shared pointer to the created NxShaderStorageBuffer instance.
         *
         * Throws:
         * - Implementation-specific exceptions if buffer creation fails.
         *
         * Notes:
         * - This function is typically implemented in a platform-specific or API-specific source file.
         */
        static std::shared_ptr<NxShaderStorageBuffer> create(unsigned int size);

        /**
         * @brief Binds the shader storage buffer to the current context.
         *
         * This method binds the shader storage buffer, making it active for subsequent operations.
         * Binding a buffer allows it to be used in shader programs for reading and writing data.
         *
         * Notes:
         * - The actual binding point may depend on the graphics API and implementation.
         */
        virtual void bind() const = 0;

        /**
         * @brief Binds the shader storage buffer to a specific binding point.
         * @param bindingLocation The binding point index to bind the buffer to.
         *
         * This method binds the shader storage buffer to a specified binding point,
         * allowing shaders to access the buffer data at that location.
         *
         * Notes:
         * - The binding location must match the one specified in the shader code.
         */
        virtual void bindBase(unsigned int bindingLocation) const = 0;

        /**
         * @brief Unbinds the shader storage buffer from the current context.
         *
         * This method unbinds the shader storage buffer, making it inactive for subsequent operations.
         * Unbinding a buffer ensures that it is no longer used in shader programs until it is bound again.
         */
        virtual void unbind() const = 0;

        /**
         * @brief Updates the data stored in the shader storage buffer.
         * @param data Pointer to the data to upload to the buffer.
         * @param size Size of the data in bytes.
         *
         * This method updates the contents of the shader storage buffer with new data.
         * The provided data pointer should point to a memory block of at least 'size' bytes.
         *
         * Notes:
         * - The size must not exceed the size of the buffer allocated during creation.
         * - The data format must match what the shader expects for correct interpretation.
         */
        virtual void setData(void *data, unsigned int size) = 0;

        virtual void appendData(void* data, unsigned int size) = 0;

        /**
         * @brief Retrieves the unique identifier of the shader storage buffer.
         * @return The unique ID of the buffer.
         *
         * This method returns an unsigned integer that uniquely identifies the shader storage buffer
         * within the graphics context. This ID can be used for debugging or advanced operations
         * that require direct access to the underlying buffer resource.
         */
        [[nodiscard]] virtual unsigned int getId() const = 0;

        /**
         * @brief Retrieves the size of the shader storage buffer.
         * @return The size of the buffer in bytes.
         *
         * This method returns the size of the shader storage buffer in bytes.
         */
        [[nodiscard]] unsigned int getSize() const { return m_size; };

        protected:
            unsigned int m_id = 0;
            unsigned int m_size = 0;
            unsigned int m_capacity = 0;
    };
} // namespace nexo::renderer
