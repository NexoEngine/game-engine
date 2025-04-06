//// AssetImporterBase.hpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        05/12/2024
//  Description: Header file for the AssetImporterBase class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Asset.hpp"
#include "AssetImporterContext.hpp"
#include "AssetImporterInput.hpp"

#include <fstream>

namespace nexo::assets {

    /**
     * @class AssetImporterBase
     *
     * @brief Interface for importing assets into the engine.
     */
    class AssetImporterBase {
        public:
            /**
 * @brief Default constructor for AssetImporterBase.
 *
 * Constructs a new AssetImporterBase object.
 */
AssetImporterBase() = default;
            /**
 * @brief Virtual destructor for AssetImporterBase.
 *
 * Defined as default to ensure that derived asset importer classes are properly cleaned up.
 */
virtual ~AssetImporterBase() = default;

            /**
             * @brief Checks if the importer can read the file at the given path.
             *
             * Implementations should open the file and check if related importer is compatible.
             * @param[in] inputVariant The input variant to check. Can be a file path or memory buffer.
             * @return True if the importer can read the file, false otherwise.
             */
            virtual bool canRead(const ImporterInputVariant& inputVariant) = 0;

            /**
             * @brief Imports an asset from a file.
             *
             * This method should be overridden by the derived class to do the actual import.
             * @warning Implementation MUST set the main asset data using AssetImporterContext::setMainAssetData() before return
             *
             * @param[in,out] ctx The context for the import.
             */
            virtual void importImpl(AssetImporterContext& ctx) = 0;

            /**
             * @brief Wraps the asset import operation with error handling.
             *
             * This method calls the asset-specific import implementation (importImpl()) to perform the asset import and verifies
             * that the main asset data is set in the provided context. It is not intended to be overridden. If importImpl() throws
             * an exception or the main asset data remains unset after the import, an error is logged.
             *
             * @param[in,out] ctx The asset importer context providing the asset location and storing the imported asset data.
             */
            void import(AssetImporterContext& ctx) noexcept
            {
                try {
                    importImpl(ctx);
                    if (ctx.getMainAsset() == nullptr) {
                        LOG(NEXO_ERROR, "Importer did not set main asset data in context");
                        return;
                    }
                } catch (const std::exception& e) {
                    // Log the error
                    LOG(NEXO_ERROR, "Failed to import asset from file '{}': {}", ctx.location.getPath(), e.what());
                }
            }
    };

} // namespace nexo::assets
