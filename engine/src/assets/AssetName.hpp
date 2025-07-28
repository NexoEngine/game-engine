//// AssetName.hpp ////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        07/12/2024
//  Description: AssetName is a wrapper of std::string to represent the name
//               of an asset. It is used to apply our own rules on naming.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ValidatedName.hpp"
#include "FilenameValidator.hpp"

namespace nexo::assets {

    struct AssetNameValidator : FilenameValidator {};

    using AssetName = ValidatedName<AssetNameValidator>;

} // namespace nexo::assets
