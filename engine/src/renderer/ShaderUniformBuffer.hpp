//// ShaderUniformBuffer.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/02/2025
//  Description: Header file for the Uniform Buffer (UBO)
//
///////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

namespace nexo::renderer {

    class NxShaderUniformBuffer {
       public:
        virtual ~NxShaderUniformBuffer() = default;

        /**
         * @brief Creates a uniform buffer of the specified size.
         * @param size Size of the buffer in bytes.
         * @return A shared pointer to the created NxShaderUniformBuffer instance.
         *
         * Throws:
         * - Implementation-specific exceptions if buffer creation fails.
         *
         * Notes:
         * - This function is typically implemented in a platform-specific or API-specific source file.
         */
        static std::shared_ptr<NxShaderUniformBuffer> create(unsigned int size);

        /**
         * @brief Binds the uniform buffer to the current context.
         *
         * This method binds the uniform buffer, making it active for subsequent operations.
         *
         * Notes:
         * - The actual binding point may depend on the graphics API and implementation.
         */
        virtual void bind() const = 0;

        /**
         * @brief Binds the uniform buffer to a specific binding point.
         * @param bindingLocation The binding point index to bind the buffer to.
         *
         * This method binds the uniform buffer to a specified binding point,
         * allowing shaders to access the buffer data at that location.
         *
         * Notes:
         * - The binding location must match the one specified in the shader code
         *   via `layout(std140, binding = N)`.
         */
        virtual void bindBase(unsigned int bindingLocation) const = 0;

        /**
         * @brief Unbinds the uniform buffer from the current context.
         *
         * This method unbinds the uniform buffer, making it inactive for subsequent operations.
         */
        virtual void unbind() const = 0;

        /**
         * @brief Updates the data stored in the uniform buffer.
         * @param data Pointer to the data to upload to the buffer.
         * @param size Size of the data in bytes.
         *
         * This method updates the contents of the uniform buffer with new data.
         * The provided data pointer should point to a memory block of at least 'size' bytes.
         *
         * Notes:
         * - The size must not exceed the size of the buffer allocated during creation.
         * - The data layout must match the std140 layout expected in the shader.
         */
        virtual void setData(void* data, unsigned int size) = 0;

        /**
         * @brief Retrieves the unique identifier of the uniform buffer.
         * @return The unique ID of the buffer.
         *
         * This method returns an unsigned integer that uniquely identifies the uniform buffer
         * within the graphics context. This ID can be used for debugging or advanced operations
         * that require direct access to the underlying buffer resource.
         */
        [[nodiscard]] virtual unsigned int getId() const = 0;
        protected:
            unsigned int m_id = 0;
            unsigned int m_capacity = 0;
    };

} // namespace nexo::renderer
