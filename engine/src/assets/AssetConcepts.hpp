//// AssetConcepts.hpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        25/11/2024
//  Description: Header file for concepts related to assets.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <concepts>

namespace nexo::assets {
    class Asset;

    template<typename T>
    concept DerivedFromAsset = std::derived_from<T, nexo::assets::Asset>;
} // namespace nexo::assets
