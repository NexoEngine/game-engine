//// AssetPackName.hpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        07/12/2024
//  Description: AssetPackName is a wrapper of std::string, derived from
//               AssetName. It is used to apply our own rules on naming.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ValidatedName.hpp"
#include "AssetName.hpp"

namespace nexo::assets {

    struct AssetPackNameValidator : FilenameValidator {};

    using AssetPackName = ValidatedName<AssetPackNameValidator>;

} // namespace nexo::assets
