//// OpenGlVertexArray.hpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for opengl vertex array class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderer/VertexArray.hpp"

namespace nexo::renderer {

    class OpenGlVertexArray final : public VertexArray {
        public:
        OpenGlVertexArray();
        ~OpenGlVertexArray() override = default;


        void bind() const override;
        void unbind() const override;

        void addVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) override;
        void setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) override;

        [[nodiscard]] const std::vector<std::shared_ptr<VertexBuffer>> &getVertexBuffers() const override;
        [[nodiscard]] const std::shared_ptr<IndexBuffer> &getIndexBuffer() const override;
        private:
        std::vector<std::shared_ptr<VertexBuffer>> _vertexBuffers;
        std::shared_ptr<IndexBuffer> _indexBuffer;

        unsigned int _id{};
    };

}
