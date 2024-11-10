//// Buffer.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for vertex buffer abstract class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <initializer_list>
#include <utility>
#include <vector>
#include <iostream>
#include <memory>

namespace nexo::renderer {

    enum class ShaderDataType {
        NONE = 0,
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        MAT3,
        MAT4,
        INT,
        INT2,
        INT3,
        INT4,
        BOOL
    };

    static unsigned int shaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::FLOAT:    return 4;  // 1 float (4 bytes)
            case ShaderDataType::FLOAT2:   return 4 * 2;  // 2 floats (8 bytes)
            case ShaderDataType::FLOAT3:   return 4 * 3;  // 3 floats (12 bytes)
            case ShaderDataType::FLOAT4:   return 4 * 4;  // 4 floats (16 bytes)
            case ShaderDataType::MAT3:     return 4 * 3 * 3;  // 3x3 matrix (36 bytes)
            case ShaderDataType::MAT4:     return 4 * 4 * 4;  // 4x4 matrix (64 bytes)
            case ShaderDataType::INT:      return 4;  // 1 int (4 bytes)
            case ShaderDataType::INT2:     return 4 * 2;  // 2 ints (8 bytes)
            case ShaderDataType::INT3:     return 4 * 3;  // 3 ints (12 bytes)
            case ShaderDataType::INT4:     return 4 * 4;  // 4 ints (16 bytes)
            case ShaderDataType::BOOL:     return 1;  // 1 byte (1 bool)
            case ShaderDataType::NONE:     return 0;  // No type, return 0
        }
        return 0; // Default case for undefined types
    }

    struct BufferElements {
        std::string name;
        ShaderDataType type{};
        unsigned int size{};
        unsigned int offset{};
        bool normalized{};

        BufferElements() = default;
        BufferElements(const ShaderDataType Type, std::string name, const bool normalized = false)
            : name(std::move(name)), type(Type), size(shaderDataTypeSize(type)), offset(0) , normalized(normalized)
        {

        }

        [[nodiscard]] unsigned int getComponentCount() const
        {
            switch(type)
            {
                case ShaderDataType::FLOAT:     return 1;
                case ShaderDataType::FLOAT2:    return 2;
                case ShaderDataType::FLOAT3:    return 3;
                case ShaderDataType::FLOAT4:    return 4;
                case ShaderDataType::INT:       return 1;
                case ShaderDataType::INT2:      return 2;
                case ShaderDataType::INT3:      return 3;
                case ShaderDataType::INT4:      return 4;
                case ShaderDataType::MAT3:      return 3 * 3;
                case ShaderDataType::MAT4:      return 4 * 4;
                case ShaderDataType::BOOL:      return 1;
                default: return -1;
            }
        }
    };

    class BufferLayout {
        public:
            BufferLayout() = default;
            BufferLayout(const std::initializer_list<BufferElements> elements)
                : _elements(elements)
            {
                calculateOffsetAndStride();
            };

            [[nodiscard]] std::vector<BufferElements> getElements() const { return _elements; };
            [[nodiscard]] unsigned int getStride() const { return _stride; };

            std::vector<BufferElements>::iterator begin() { return _elements.begin(); };
            std::vector<BufferElements>::iterator end() { return _elements.end(); };
            [[nodiscard]] std::vector<BufferElements>::const_iterator begin() const { return _elements.begin(); };
            [[nodiscard]] std::vector<BufferElements>::const_iterator end() const { return _elements.end(); };
        private:
            std::vector<BufferElements> _elements;
            unsigned int _stride{};

            void calculateOffsetAndStride()
            {
                unsigned int offset = 0;
                _stride = 0;
                for (auto &element : _elements)
                {
                    element.offset = offset;
                    offset += element.size;
                    _stride += element.size;
                }
            }
    };

    class VertexBuffer {
        public:
            virtual ~VertexBuffer() = default;

            virtual void bind() const = 0;
            virtual void unbind() const = 0;

            virtual void setLayout(const BufferLayout &layout) = 0;
            [[nodiscard]] virtual const BufferLayout getLayout() const = 0;

            virtual void setData(void *data, unsigned int size) = 0;
    };

    class IndexBuffer {
        public:
            virtual ~IndexBuffer() = default;

            virtual void bind() const  = 0;
            virtual void unbind() const = 0;

            [[nodiscard]] virtual unsigned int getCount() const = 0;
    };

    std::shared_ptr<VertexBuffer> createVertexBuffer(float *vertices, unsigned int size);
    std::shared_ptr<VertexBuffer> createVertexBuffer(unsigned int size);
    std::shared_ptr<IndexBuffer> createIndexBuffer(unsigned int *indices, unsigned int count);


}
