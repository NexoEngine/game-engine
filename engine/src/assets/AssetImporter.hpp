//// AssetImporter.hpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        05/12/2024
//  Description: Header file for the AssetImporter class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Asset.hpp"

#include <fstream>

namespace nexo::assets {

    /**
     * @class AssetImporter
     *
     * @brief Interface for importing assets into the engine.
     */
    class AssetImporter {
        public:
            AssetImporter() = default;
            virtual ~AssetImporter() = default;

            /**
             * @brief Checks if the importer can read the file at the given path.
             *
             * Implementations should open the file and check if related importer is compatible.
             * @param fsPath The path to the file to check.
             * @return True if the importer can read the file, false otherwise.
             */
            virtual bool canRead(std::filesystem::path fsPath) = 0;

            /**
             * @brief Imports an asset from a file.
             *
             * This method should be overridden by the derived class to do the actual import.
             *
             * @param fsPath The path to the file to import.
             * @return The imported asset.
             */
            virtual IAsset *importImpl(std::filesystem::path fsPath) = 0;

            /**
             * @brief Imports an asset from a file.
             *
             * This method is not intended to be overridden. Implement importImpl() to do the import.
             * This method is a wrapper of importImpl() that for example catches exceptions thrown by importImpl().
             *
             * @param fsPath The path to the file to import.
             * @return The imported asset. Or nullptr if the import failed.
             */
            IAsset *import(std::filesystem::path fsPath) noexcept
            {
                try {
                    return importImpl(std::move(fsPath));
                } catch (const std::exception& e) {
                    // Log the error
                    LOG(NEXO_ERROR, "Failed to import asset from file '{}': {}", fsPath.string(), e.what());
                    return nullptr;
                }
            }
    };

} // namespace nexo::assets
