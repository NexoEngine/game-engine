//// AssetName.hpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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
