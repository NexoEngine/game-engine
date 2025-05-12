//// TextureImporter.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        24/02/2025
//  Description: Header file for the TextureImporter class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "assets/AssetImporterBase.hpp"

namespace nexo::assets {

    class TextureImporter final : public AssetImporterBase {
        public:
            TextureImporter() = default;
            ~TextureImporter() override = default;

            bool canRead(const ImporterInputVariant& inputVariant) override;
            void importImpl(AssetImporterContext& ctx) override;
        protected:
            static bool canReadMemory(const ImporterMemoryInput& input);
            static bool canReadFile(const ImporterFileInput& input);
    };

} // namespace nexo::assets
