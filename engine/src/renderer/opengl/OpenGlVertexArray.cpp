//// OpenGlVertexArray.cpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for opengl vertex array class
//
///////////////////////////////////////////////////////////////////////////////

#include "OpenGlVertexArray.hpp"
#include "Logger.hpp"

#include <glad/glad.h>

namespace nexo::renderer {

    static GLenum shaderDataTypeToOpenGltype(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::FLOAT: return GL_FLOAT;
            case ShaderDataType::FLOAT2: return GL_FLOAT;
            case ShaderDataType::FLOAT3: return GL_FLOAT;
            case ShaderDataType::FLOAT4: return GL_FLOAT;
            case ShaderDataType::INT: return GL_INT;
            case ShaderDataType::INT2: return GL_INT;
            case ShaderDataType::INT3: return GL_INT;
            case ShaderDataType::INT4: return GL_INT;
            case ShaderDataType::MAT3: return GL_FLOAT;
            case ShaderDataType::MAT4: return GL_FLOAT;
            case ShaderDataType::BOOL: return GL_BOOL;
            default: return -1;
        }
    }

    OpenGlVertexArray::OpenGlVertexArray()
    {
        glGenVertexArrays(1, &_id);
    }

    void OpenGlVertexArray::bind() const
    {
        glBindVertexArray(_id);
    }

    void OpenGlVertexArray::unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGlVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer)
    {
        glBindVertexArray(_id);
        vertexBuffer->bind();

        if (vertexBuffer->getLayout().getElements().empty()) {
            LOG(NEXO_ERROR, "The vertex buffer has no layout");
            return;
        }

        unsigned int index = 0;
        const auto& layout = vertexBuffer->getLayout();
        for (const auto &element : layout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                element.getComponentCount(),
                shaderDataTypeToOpenGltype(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                layout.getStride(),
                (const void *)(uintptr_t)element.offset
            );
            index++;
        }
        _vertexBuffers.push_back(vertexBuffer);
    }

    void OpenGlVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
    {
        glBindVertexArray(_id);
        indexBuffer->bind();

        _indexBuffer = indexBuffer;
    }

    const std::vector<std::shared_ptr<VertexBuffer>> &OpenGlVertexArray::getVertexBuffers() const
    {
        return _vertexBuffers;
    }

    const std::shared_ptr<IndexBuffer> &OpenGlVertexArray::getIndexBuffer() const
    {
        return _indexBuffer;
    }
}