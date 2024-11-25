//// Texture.hpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for texture abstract classes
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <memory>

namespace nexo::renderer {

    class Texture {
        public:
            virtual ~Texture() = default;

            [[nodiscard]] virtual unsigned int getWidth() const = 0;
            [[nodiscard]] virtual unsigned int getHeight() const = 0;
            virtual unsigned int getMaxTextureSize() const = 0;

            [[nodiscard]] virtual unsigned int getId() const = 0;

            virtual void bind(unsigned int slot = 0) const = 0;
            virtual void unbind(unsigned int slot = 0) const = 0;

            virtual void setData(void *data, unsigned int size) = 0;

            virtual bool operator==(const Texture &other) const { return this->getId() == other.getId(); };
    };

    class Texture2D :  public Texture {
        public:
            static std::shared_ptr<Texture2D> create(unsigned int width, unsigned int height);
            static std::shared_ptr<Texture2D> create(const std::string &path);
    };

}