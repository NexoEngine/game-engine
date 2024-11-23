//// VertexArray.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for vertex array abstract class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Buffer.hpp"

namespace nexo::renderer {

    class VertexArray {
        public:
            virtual ~VertexArray() = default;

            virtual void bind() const = 0;
            virtual void unbind() const = 0;

            virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) = 0;
            virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) = 0;

            [[nodiscard]] virtual const std::vector<std::shared_ptr<VertexBuffer>> &getVertexBuffers() const = 0;
            [[nodiscard]] virtual const std::shared_ptr<IndexBuffer> &getIndexBuffer() const = 0;

            virtual unsigned int getId() const = 0;
    };

    std::shared_ptr<VertexArray> createVertexArray();
}
