//// OpenGlVertexArray.cpp ////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
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
     * @brief Converts an `NxShaderDataType` enum value to the corresponding OpenGL type.
     *
     * @param type The shader data type to convert.
     * @return The OpenGL equivalent type (e.g., `GL_FLOAT`).
     *
     * Example:
     * ```cpp
     * GLenum glType = nxShaderDataTypeToOpenGltype(NxShaderDataType::FLOAT3);
     * ```
     **/
    static GLenum nxShaderDataTypeToOpenGltype(const NxShaderDataType type)
    {
        switch (type) {
            using enum NxShaderDataType;
            case FLOAT:
            case FLOAT2:
            case FLOAT3:
            case FLOAT4:
            case MAT3:
            case MAT4:
                return GL_FLOAT;
            case INT:
            case INT2:
            case INT3:
            case INT4:
                return GL_INT;
            case BOOL:
                return GL_BOOL;
            default:
                return 0;
        }
    }

    /**
     * @brief Checks if the given `NxShaderDataType` is an integer type.
     *
     * @param type The shader data type to check.
     * @return `true` if the type is an integer type, `false` otherwise.
     *
     * Example:
     * ```cpp
     * bool isIntType = isInt(NxShaderDataType::INT3); // returns true
     * ```
     **/
    static bool isInt(const NxShaderDataType type)
    {
        switch (type) {
            using enum NxShaderDataType;
            case INT:
            case INT2:
            case INT3:
            case INT4:
            case BOOL:
                return true;
            default:
                return false;
        }
    }

    NxOpenGlVertexArray::NxOpenGlVertexArray()
    {
        glGenVertexArrays(1, &_id);
    }

    void NxOpenGlVertexArray::bind() const
    {
        glBindVertexArray(_id);
    }

    void NxOpenGlVertexArray::unbind() const
    {
        glBindVertexArray(0);
    }

    void NxOpenGlVertexArray::addVertexBuffer(const std::shared_ptr<NxVertexBuffer> &vertexBuffer)
    {
        if (!vertexBuffer) THROW_EXCEPTION(NxInvalidValue, "OPENGL", "Vertex buffer is null");
        glBindVertexArray(_id);
        vertexBuffer->bind();

        if (vertexBuffer->getLayout().getElements().empty()) THROW_EXCEPTION(NxBufferLayoutEmpty, "OPENGL");

        auto index = static_cast<unsigned int>(
            !_vertexBuffers.empty() ? _vertexBuffers.back()->getLayout().getElements().size() : 0);
        const auto &layout = vertexBuffer->getLayout();
        for (const auto &element : layout) {
            glEnableVertexAttribArray(index);
            if (isInt(element.type)) {
                glVertexAttribIPointer(index, static_cast<int>(element.getComponentCount()),
                                       nxShaderDataTypeToOpenGltype(element.type), static_cast<int>(layout.getStride()),
                                       static_cast<const void *>(reinterpret_cast<uintptr_t *>(element.offset)));

            } else {
                glVertexAttribPointer(index, static_cast<int>(element.getComponentCount()),
                                      nxShaderDataTypeToOpenGltype(element.type),
                                      element.normalized ? GL_TRUE : GL_FALSE, static_cast<int>(layout.getStride()),
                                      reinterpret_cast<const void *>(static_cast<uintptr_t>(element.offset)));
            }
            index++;
        }
        _vertexBuffers.push_back(vertexBuffer);
    }

    void NxOpenGlVertexArray::setIndexBuffer(const std::shared_ptr<NxIndexBuffer> &indexBuffer)
    {
        if (!indexBuffer) THROW_EXCEPTION(NxInvalidValue, "OPENGL", "Index buffer cannot be null");
        glBindVertexArray(_id);
        indexBuffer->bind();

        _indexBuffer = indexBuffer;
    }

    const std::vector<std::shared_ptr<NxVertexBuffer>> &NxOpenGlVertexArray::getVertexBuffers() const
    {
        return _vertexBuffers;
    }

    const std::shared_ptr<NxIndexBuffer> &NxOpenGlVertexArray::getIndexBuffer() const
    {
        return _indexBuffer;
    }

    unsigned int NxOpenGlVertexArray::getId() const
    {
        return _id;
    }

} // namespace nexo::renderer
