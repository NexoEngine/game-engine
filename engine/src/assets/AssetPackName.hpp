//// AssetPackName.hpp ////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
