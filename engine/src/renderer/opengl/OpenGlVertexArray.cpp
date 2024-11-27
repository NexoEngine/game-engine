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
#include "renderer/RendererExceptions.hpp"

#include <glad/glad.h>

namespace nexo::renderer {

    /**
    * @brief Converts a `ShaderDataType` enum value to the corresponding OpenGL type.
    *
    * @param type The shader data type to convert.
    * @return The OpenGL equivalent type (e.g., `GL_FLOAT`).
    *
    * Example:
    * ```cpp
    * GLenum glType = shaderDataTypeToOpenGltype(ShaderDataType::FLOAT3);
    * ```
    */
    static GLenum shaderDataTypeToOpenGltype(const ShaderDataType type)
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
            default: return 0;
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
        if (!vertexBuffer)
            THROW_EXCEPTION(InvalidValue, "OPENGL", "Vertex buffer is null");
        glBindVertexArray(_id);
        vertexBuffer->bind();

        if (vertexBuffer->getLayout().getElements().empty())
            THROW_EXCEPTION(BufferLayoutEmpty, "OPENGL");

        unsigned int index = static_cast<unsigned int>(_vertexBuffers.size() > 0
            ? _vertexBuffers.back()->getLayout().getElements().size()
            : 0);
        const auto& layout = vertexBuffer->getLayout();
        for (const auto &element : layout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                static_cast<int>(element.getComponentCount()),
                shaderDataTypeToOpenGltype(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                static_cast<int>(layout.getStride()),
                reinterpret_cast<const void *>(static_cast<uintptr_t>(element.offset))
            );
            index++;
        }
        _vertexBuffers.push_back(vertexBuffer);
    }

    void OpenGlVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
    {
        if (!indexBuffer)
            THROW_EXCEPTION(InvalidValue, "OPENGL", "Index buffer cannot be null");
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

    unsigned int OpenGlVertexArray::getId() const
    {
        return _id;
    }

}
