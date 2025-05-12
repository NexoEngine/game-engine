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

			static std::shared_ptr<NxShaderStorageBuffer> create(unsigned int size);

			virtual void bind() const = 0;
			virtual void bindBase(unsigned int bindingLocation) const = 0;
			virtual void unbind() const = 0;

			virtual void setData(void *data, unsigned int size) = 0;
			[[nodiscard]] virtual unsigned int getId() const = 0;
	};
}
