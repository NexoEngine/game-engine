//// ShaderStorageBuffer.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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

			static std::shared_ptr<NxShaderStorageBuffer> create(unsigned int size);

			virtual void bind() const = 0;
			virtual void bindBase(unsigned int bindingLocation) const = 0;
			virtual void unbind() const = 0;

			virtual void setData(void *data, unsigned int size) = 0;
			[[nodiscard]] virtual unsigned int getId() const = 0;
	};
}
