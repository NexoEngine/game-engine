//// OpenGlShaderStorageBuffer.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/02/2025
//  Description: Header file for the opengl implementation of SSBOs
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "renderer/ShaderStorageBuffer.hpp"

namespace nexo::renderer {
    class NxOpenGlShaderStorageBuffer final : public NxShaderStorageBuffer {
       public:
        /**
         * @brief Constructs an OpenGL Shader Storage Buffer Object (SSBO) with the specified size.
         * @param size The size in bytes of the buffer to allocate.
         */
        explicit NxOpenGlShaderStorageBuffer(unsigned int size);

        /**
         * @brief Destructor for the OpenGL SSBO.
         */
        ~NxOpenGlShaderStorageBuffer() override = default;

        /**
         * @brief Binds the SSBO to the current OpenGL context.
         */
        void bind() const override;

        /**
         * @brief Binds the SSBO to a specific binding location.
         * @param bindingLocation The binding point index.
         */
        void bindBase(unsigned int bindingLocation) const override;

        /**
         * @brief Unbinds the SSBO from the current OpenGL context.
         */
        void unbind() const override;

        /**
         * @brief Sets the data of the SSBO.
         * @param data Pointer to the data to upload.
         * @param size Size in bytes of the data.
         */
        void setData(void* data, unsigned int size) override;

        void appendData(void* data, unsigned int size) override;

        /**
         * @brief Returns the OpenGL buffer object ID.
         * @return The OpenGL buffer ID.
         */
        [[nodiscard]] unsigned int getId() const override
        {
            return m_id;
        };
    };
} // namespace nexo::renderer
