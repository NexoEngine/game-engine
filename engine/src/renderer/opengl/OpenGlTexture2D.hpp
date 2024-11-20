//// OpenGlTexture2D.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for open gl texture class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderer/Texture.hpp"
#include <glad/glad.h>

namespace nexo::renderer {
    class OpenGlTexture2D final : public Texture2D {
        public:
            explicit OpenGlTexture2D(const std::string &path);
            OpenGlTexture2D(unsigned int width, unsigned int height);
            ~OpenGlTexture2D() override;

            [[nodiscard]] unsigned int getWidth() const override {return m_width;};
            [[nodiscard]] unsigned int getHeight() const override {return m_height;};

            [[nodiscard]] unsigned int getId() const override {return m_id;};

            void bind(unsigned int slot = 0) const override;
            void setData(void *data, unsigned int size) override;

            bool operator==(const Texture &other) const override { return m_id == other.getId(); };
        private:
            std::string m_path;
            unsigned int m_width;
            unsigned int m_height;
            unsigned int m_id{};
            GLenum m_internalFormat;
            GLenum m_dataFormat;
    };
}