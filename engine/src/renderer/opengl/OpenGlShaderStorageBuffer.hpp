//// OpenGlShaderStorageBuffer.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
		explicit NxOpenGlShaderStorageBuffer(unsigned int size);
		~NxOpenGlShaderStorageBuffer() override = default;

		void bind() const override;
		void bindBase(unsigned int bindingLocation) const override;
		void unbind() const override;

		void setData(void* data, unsigned int size) override;

		[[nodiscard]] unsigned int getId() const override { return m_id; };

	private:
		unsigned int m_id{};
	};
}
