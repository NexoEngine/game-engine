//// Shader.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for shader abstract class and shader library
//
///////////////////////////////////////////////////////////////////////////////
#include "Shader.hpp"
#include "renderer/RendererExceptions.hpp"
#include "Logger.hpp"
#ifdef NX_GRAPHICS_API_OPENGL
    #include "opengl/OpenGlShader.hpp"
#endif

#include <fstream>

namespace nexo::renderer {

    std::shared_ptr<NxShader> NxShader::create(const std::string &path)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlShader>(path);
        #endif
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");

    }

    std::shared_ptr<NxShader> NxShader::create(const std::string& name, const std::string &vertexSource, const std::string &fragmentSource)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlShader>(name, vertexSource, fragmentSource);
        #endif
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");

    }

    std::string NxShader::readFile(const std::string &filepath)
    {
        std::string result;
        if (std::ifstream in(filepath, std::ios::in | std::ios::binary); in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
            return result;
        }
        THROW_EXCEPTION(NxFileNotFoundException, filepath);
    }

    void NxShader::addStorageBuffer(const std::shared_ptr<NxShaderStorageBuffer> &buffer)
    {
        m_storageBuffers.push_back(buffer);
    }

    void NxShader::setStorageBufferData(unsigned int index, void *data, unsigned int size)
    {
        if (index >= m_storageBuffers.size())
            THROW_EXCEPTION(NxOutOfRangeException, index, m_storageBuffers.size());
        m_storageBuffers[index]->setData(data, size);
    }
}
