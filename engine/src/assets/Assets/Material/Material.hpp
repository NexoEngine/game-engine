//// Material.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        24/02/2025
//  Description: Header file for the Material class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "assets/Asset.hpp"

#include <components/Shapes3D.hpp>

namespace nexo::assets {

    /**
     * @class Material
     *
     * @brief Represents a material asset.
     */
    class Material final : public Asset<components::Material, AssetType::MATERIAL> {
        public:
            Material() = default;

            ~Material() override = default;
    };

}
