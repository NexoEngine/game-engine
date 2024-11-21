//// OpenGlBuffer.hpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the opengl buffers (vertex and index)
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderer/Buffer.hpp"

namespace nexo::renderer {

    class OpenGlVertexBuffer final : public VertexBuffer {
        public:
        OpenGlVertexBuffer(const float *vertices, unsigned int size);
        explicit OpenGlVertexBuffer(unsigned int size);
        ~OpenGlVertexBuffer() override;

        void bind() const override;
        void unbind() const override;
        void setLayout(const BufferLayout &layout) override { _layout = layout; };
        [[nodiscard]] const BufferLayout getLayout() const override { return _layout; };

        void setData(void *data, unsigned int size) override;
        private:
        unsigned int _id{};
        BufferLayout _layout;
    };

    class OpenGlIndexBuffer final : public IndexBuffer {
        public:
        OpenGlIndexBuffer();
        ~OpenGlIndexBuffer() override;

        void bind() const override;
        void unbind() const override;

        void setData(unsigned int *indices, unsigned int count) override;

        [[nodiscard]] unsigned int getCount() const override;
        private:
        unsigned int _id{};
        unsigned int _count;
    };

}
