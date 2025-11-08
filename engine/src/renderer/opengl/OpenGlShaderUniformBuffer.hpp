//// OpenGlShaderUniformBuffer.hpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/02/2025
//  Description: Header file for the OpenGL implementation of the UBO
//
///////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ShaderUniformBuffer.hpp"

namespace nexo::renderer {

    class NxOpenGlShaderUniformBuffer : public NxShaderUniformBuffer {
       public:
        explicit NxOpenGlShaderUniformBuffer(unsigned int size);
        ~NxOpenGlShaderUniformBuffer() override = default;

        void bind() const override;
        void bindBase(unsigned int bindingLocation) const override;
        void unbind() const override;
        void setData(void* data, unsigned int size) override;
        [[nodiscard]] unsigned int getId() const override { return m_id; }
    };

} // namespace nexo::renderer
