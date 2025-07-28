//// TextureImporter.hpp //////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
