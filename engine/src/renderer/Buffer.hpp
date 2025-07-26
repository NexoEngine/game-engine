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

    /**
     * @enum NxShaderDataType
     * @brief Enum representing the various data types supported in shaders.
     *
     * NxShaderDataType is used to define the type of data stored in a buffer element,
     * such as float, integer, matrix, or boolean types. This enum ensures consistent
     * representation and handling of data types in vertex and index buffers.
     *
     * Types:
     * - NONE: No specific type, used as a default.
     * - FLOAT, FLOAT2, FLOAT3, FLOAT4: Represents one or more floating-point values.
     * - MAT3, MAT4: Represents 3x3 or 4x4 matrices.
     * - INT, INT2, INT3, INT4: Represents one or more integer values.
     * - BOOL: Represents a boolean value.
     */
    enum class NxShaderDataType {
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

    /**
     * @brief Returns the size (in bytes) of a given NxShaderDataType.
     *
     * This function calculates the memory size required for a specific NxShaderDataType.
     * It supports floats, integers, matrices, and boolean types.
     *
     * @param type The NxShaderDataType whose size is to be calculated.
     * @return The size in bytes of the provided NxShaderDataType.
     *
     * Example:
     * - FLOAT3 will return 12 (3 floats, 4 bytes each).
     * - MAT4 will return 64 (4x4 matrix of floats).
     */
    static unsigned int shaderDataTypeSize(const NxShaderDataType type)
    {
        switch (type)
        {
            case NxShaderDataType::FLOAT:    return 4;  // 1 float (4 bytes)
            case NxShaderDataType::FLOAT2:   return 4 * 2;  // 2 floats (8 bytes)
            case NxShaderDataType::FLOAT3:   return 4 * 3;  // 3 floats (12 bytes)
            case NxShaderDataType::FLOAT4:   return 4 * 4;  // 4 floats (16 bytes)
            case NxShaderDataType::MAT3:     return 4 * 3 * 3;  // 3x3 matrix (36 bytes)
            case NxShaderDataType::MAT4:     return 4 * 4 * 4;  // 4x4 matrix (64 bytes)
            case NxShaderDataType::INT:      return 4;  // 1 int (4 bytes)
            case NxShaderDataType::INT2:     return 4 * 2;  // 2 ints (8 bytes)
            case NxShaderDataType::INT3:     return 4 * 3;  // 3 ints (12 bytes)
            case NxShaderDataType::INT4:     return 4 * 4;  // 4 ints (16 bytes)
            case NxShaderDataType::BOOL:     return 1;  // 1 byte (1 bool)
            case NxShaderDataType::NONE:     return 0;  // No type, return 0
        }
        return 0; // Default case for undefined types
    }

    /**
     * @struct NxBufferElements
     * @brief Represents an individual element in a buffer layout.
     *
     * Each buffer element specifies its name, type, size, and offset within a buffer.
     * This struct is used to define the layout of data in a vertex buffer.
     *
     * Members:
     * - @param name The name of the buffer element (e.g., "position" or "color").
     * - @param type The data type of the element (e.g., FLOAT3 for a 3D position).
     * - @param size The size of the element in bytes, calculated from the data type.
     * - @param offset The offset (in bytes) of the element within the buffer.
     * - @param normalized Indicates whether the data should be normalized (e.g., for colors).
     *
     * Functions:
     * - @return getComponentCount() Retrieves the number of components (e.g., FLOAT3 = 3).
     */
    struct NxBufferElements {
        std::string name;
        NxShaderDataType type{};
        unsigned int size{};
        unsigned int offset{};
        bool normalized{};

        NxBufferElements() = default;
        NxBufferElements(const NxShaderDataType Type, std::string name, const bool normalized = false)
            : name(std::move(name)), type(Type), size(shaderDataTypeSize(type)), offset(0) , normalized(normalized)
        {

        }

        [[nodiscard]] unsigned int getComponentCount() const
        {
            switch(type)
            {
                case NxShaderDataType::FLOAT:     return 1;
                case NxShaderDataType::FLOAT2:    return 2;
                case NxShaderDataType::FLOAT3:    return 3;
                case NxShaderDataType::FLOAT4:    return 4;
                case NxShaderDataType::INT:       return 1;
                case NxShaderDataType::INT2:      return 2;
                case NxShaderDataType::INT3:      return 3;
                case NxShaderDataType::INT4:      return 4;
                case NxShaderDataType::MAT3:      return 3 * 3;
                case NxShaderDataType::MAT4:      return 4 * 4;
                case NxShaderDataType::BOOL:      return 1;
                default: return -1;
            }
        }
    };

    /**
     * @class NxBufferLayout
     * @brief Defines the structure and layout of elements in a vertex buffer.
     *
     * A NxBufferLayout is a collection of BufferElements, each specifying a data type,
     * size, and offset. The layout is essential for ensuring correct binding and
     * rendering of vertex attributes in a graphics pipeline.
     *
     * Members:
     * - @param _elements A vector containing all elements of the layout.
     * - @param _stride The total size (in bytes) of one vertex in the layout.
     *
     * Functions:
     * - @constructor NxBufferLayout(const std::initializer_list<BufferElements> elements)
     *   Initializes the layout with a list of buffer elements and calculates offsets/stride.
     *
     * - @return getElements() Retrieves the list of BufferElements.
     * - @return getStride() Retrieves the stride of the layout (sum of all element sizes).
     * - @function calculateOffsetAndStride() Calculates the offset and stride for all elements.
     *
     * Iterators:
     * - Supports begin() and end() iterators for easy iteration over elements.
     */
    class NxBufferLayout {
        public:
            NxBufferLayout() = default;
            NxBufferLayout(const std::initializer_list<NxBufferElements> elements)
                : _elements(elements)
            {
                calculateOffsetAndStride();
            };

            [[nodiscard]] std::vector<NxBufferElements> getElements() const { return _elements; };
            [[nodiscard]] unsigned int getStride() const { return _stride; };

            std::vector<NxBufferElements>::iterator begin() { return _elements.begin(); };
            std::vector<NxBufferElements>::iterator end() { return _elements.end(); };
            [[nodiscard]] std::vector<NxBufferElements>::const_iterator begin() const { return _elements.begin(); };
            [[nodiscard]] std::vector<NxBufferElements>::const_iterator end() const { return _elements.end(); };
        private:
            std::vector<NxBufferElements> _elements;
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

    /**
     * @class NxVertexBuffer
     * @brief Abstract class representing a vertex buffer in the graphics pipeline.
     *
     * A vertex buffer is a GPU memory buffer that stores per-vertex attributes, such as
     * positions, colors, normals, and texture coordinates. This abstract class defines
     * the interface for creating, binding, and managing vertex buffers, allowing for
     * implementation across various graphics APIs.
     */
    class NxVertexBuffer {
        public:
            /**
             * @brief Destroys the vertex buffer.
             *
             * This virtual destructor ensures that derived classes properly release any
             * resources associated with the vertex buffer, such as GPU memory.
             *
             * Usage:
             * - Automatically called when a VertexBuffer object goes out of scope.
             */
            virtual ~NxVertexBuffer() = default;

            /**
             * @brief Binds the vertex buffer as the active buffer in the graphics pipeline.
             *
             * Binding the vertex buffer ensures that subsequent rendering commands will use
             * the data stored in this buffer.
             *
             * Pure Virtual Function:
             * - Must be implemented by platform-specific subclasses (e.g., NxOpenGlVertexBuffer).
             */
            virtual void bind() const = 0;

            /**
             * @brief Unbinds the currently bound vertex buffer.
             *
             * Unbinding the vertex buffer ensures that no unintended operations are performed
             * on the buffer. This is optional but useful for debugging or ensuring clean state management.
             *
             * Pure Virtual Function:
             * - Must be implemented by platform-specific subclasses (e.g., NxOpenGlVertexBuffer).
             */
            virtual void unbind() const = 0;

            /**
             * @brief Sets the layout of the vertex buffer.
             *
             * The layout defines the structure of the data stored in the buffer (e.g., positions,
             * normals, colors) and how they are passed to the vertex shader.
             *
             * @param layout The NxBufferLayout object defining the structure of the buffer data.
             *
             * Pure Virtual Function:
             * - Must be implemented by platform-specific subclasses.
             */
            virtual void setLayout(const NxBufferLayout &layout) = 0;

            /**
             * @brief Retrieves the layout of the vertex buffer.
             *
             * Provides information about the data structure stored in the buffer, including
             * element types, sizes, and offsets.
             *
             * @return The NxBufferLayout object associated with this vertex buffer.
             *
             * Pure Virtual Function:
             * - Must be implemented by platform-specific subclasses.
             */
            [[nodiscard]] virtual NxBufferLayout getLayout() const = 0;

            /**
             * @brief Uploads new data to the vertex buffer.
             *
             * This method replaces the contents of the vertex buffer with new data. It is
             * typically used for dynamic buffers where the data changes frequently.
             *
             * @param data Pointer to the new data to upload.
             * @param size The size (in bytes) of the new data.
             *
             * Pure Virtual Function:
             * - Must be implemented by platform-specific subclasses.
             */
            virtual void setData(void *data, unsigned int size) = 0;

            [[nodiscard]] virtual unsigned int getId() const = 0;
    };

    /**
     * @class NxIndexBuffer
     * @brief Abstract class representing an index buffer in the graphics pipeline.
     *
     * An index buffer stores indices into a vertex buffer, allowing for efficient reuse
     * of vertex data. This class provides an abstract interface for creating, binding,
     * and managing index buffers, enabling compatibility with multiple graphics APIs.
     */
    class NxIndexBuffer {
        public:
            /**
            * @brief Destroys the index buffer.
            *
            * This virtual destructor ensures that derived classes properly release any
            * resources associated with the index buffer, such as GPU memory.
            *
            * Usage:
            * - Automatically called when an IndexBuffer object goes out of scope.
            */
            virtual ~NxIndexBuffer() = default;

            /**
             * @brief Binds the index buffer as the active buffer in the graphics pipeline.
             *
             * Binding the index buffer ensures that subsequent rendering commands will use
             * the indices stored in this buffer to draw primitives.
             *
             * Pure Virtual Function:
             * - Must be implemented by platform-specific subclasses (e.g., OpenGLIndexBuffer).
             */
            virtual void bind() const  = 0;

            /**
             * @brief Unbinds the currently bound index buffer.
             *
             * Unbinding the index buffer ensures that no unintended operations are performed
             * on the buffer. This is optional but useful for debugging or ensuring clean state management.
             *
             * Pure Virtual Function:
             * - Must be implemented by platform-specific subclasses (e.g., OpenGLIndexBuffer).
             */
            virtual void unbind() const = 0;

            /**
             * @brief Uploads new index data to the index buffer.
             *
             * This method replaces the contents of the index buffer with new data. It is
             * commonly used to define how vertices are connected to form primitives.
             *
             * @param data Pointer to the array of indices to upload.
             * @param size The number of indices to upload.
             *
             * Pure Virtual Function:
             * - Must be implemented by platform-specific subclasses.
             */
            virtual void setData(unsigned int *data, unsigned int size) = 0;

            /**
             * @brief Retrieves the number of indices in the index buffer.
             *
             * The count specifies how many indices are stored in the buffer, which is
             * necessary for rendering indexed primitives.
             *
             * @return The number of indices in the buffer.
             *
             * Pure Virtual Function:
             * - Must be implemented by platform-specific subclasses.
             */
            [[nodiscard]] virtual unsigned int getCount() const = 0;

            [[nodiscard]] virtual unsigned int getId() const = 0;
    };

    /**
     * @function createVertexBuffer(float *vertices, unsigned int size)
     * @brief Creates a vertex buffer with the specified data and size.
     *
     * This function abstracts the creation of a vertex buffer, delegating the actual
     * implementation to the active graphics API (e.g., OpenGL).
     *
     * @param vertices Pointer to the vertex data.
     * @param size The size (in bytes) of the vertex data.
     * @return A shared pointer to the created VertexBuffer instance.
     *
     * Throws:
     * - NxUnknownGraphicsApi exception if no graphics API is defined.
     */
    std::shared_ptr<NxVertexBuffer> createVertexBuffer(float *vertices, unsigned int size);

    /**
     * @function createVertexBuffer(unsigned int size)
     * @brief Creates an empty vertex buffer with a specified size.
     *
     * Useful for creating buffers that will be populated later.
     *
     * @param size The size (in bytes) of the buffer.
     * @return A shared pointer to the created VertexBuffer instance.
     *
     * Throws:
     * - NxUnknownGraphicsApi exception if no graphics API is defined.
     */
    std::shared_ptr<NxVertexBuffer> createVertexBuffer(unsigned int size);

    /**
     * @function createIndexBuffer()
     * @brief Creates an index buffer.
     *
     * This function abstracts the creation of an index buffer, delegating the actual
     * implementation to the active graphics API (e.g., OpenGL).
     *
     * @return A shared pointer to the created IndexBuffer instance.
     *
     * Throws:
     * - NxUnknownGraphicsApi exception if no graphics API is defined.
     */
    std::shared_ptr<NxIndexBuffer> createIndexBuffer();


}
