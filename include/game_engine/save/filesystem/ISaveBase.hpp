//// ISaveBase.hpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/11/2024
//  Description: Definition of engine file wrapper.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <filesystem>

namespace engine::save {

    class ISaveBase {
    public:
        virtual ~ISaveBase() = default;

        virtual void save() = 0;
		virtual void load() = 0;

        virtual void setPath(std::filesystem::path path) = 0;
        virtual void resetPath() = 0;
    };

} // namespace engine::save
