//// AssetImporterBase.cpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        19/11/2025
//  Description: Implementation file for the AssetImporterBase class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <fstream>
#include <iomanip>

#include "AssetImporterBase.hpp"
#include "AssetImporterContext.hpp"

#include "Logger.hpp"

namespace nexo::assets {

    void AssetImporterBase::import(AssetImporterContext& ctx) noexcept
    {
        try {
            importImpl(ctx);
            if (ctx.getMainAsset() == nullptr) {
                LOG(NEXO_ERROR, "Importer did not set main asset data in context");
                return;
            }
        } catch (const std::exception& e) {
            // Log the error
            if (std::holds_alternative<ImporterFileInput>(ctx.input))
                LOG(NEXO_ERROR, "Failed to import asset {} from file {}: {}",
                    std::quoted(ctx.location.getFullLocation()),
                    std::quoted(std::get<ImporterFileInput>(ctx.input).filePath.generic_string()), e.what());
            else if (std::holds_alternative<ImporterMemoryInput>(ctx.input))
                LOG(NEXO_ERROR, "Failed to import asset {} from memory: {}",
                    std::quoted(ctx.location.getFullLocation()), e.what());
            else
                LOG(NEXO_ERROR, "Failed to import asset {}: {}", std::quoted(ctx.location.getFullLocation()), e.what());
        }
    }

} // namespace nexo::assets
