//// OpenGlShaderStorageBuffer.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/02/2025
//  Description: Source file for the opengl implementation of the SSBOs
//
///////////////////////////////////////////////////////////////////////////////

#include <glad/glad.h>
#include "OpenGlShaderStorageBuffer.hpp"

namespace nexo::renderer {
	NxOpenGlShaderStorageBuffer::NxOpenGlShaderStorageBuffer(const unsigned int size)
	{
		glCreateBuffers(1, &m_id);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void NxOpenGlShaderStorageBuffer::bind() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
	}

	void NxOpenGlShaderStorageBuffer::bindBase(const unsigned int bindingLocation) const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingLocation, m_id);
	}

	void NxOpenGlShaderStorageBuffer::unbind() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void NxOpenGlShaderStorageBuffer::setData(void* data, const unsigned int size)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}
