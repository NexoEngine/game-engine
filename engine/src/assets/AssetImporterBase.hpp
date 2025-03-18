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
 * @brief Default constructor for the AssetImporterBase class.
 *
 * Initializes a new instance of the AssetImporterBase, serving as the base
 * interface for asset importers.
 */
AssetImporterBase() = default;
            /**
 * @brief Virtual destructor for AssetImporterBase.
 *
 * Ensures proper cleanup of resources in derived classes when an object is deleted via a base class pointer.
 */
virtual ~AssetImporterBase() = default;

            /**
             * @brief Checks if the importer can read the file at the given path.
             *
             * Implementations should open the file and check if related importer is compatible.
             * @param inputVariant The input variant to check. Can be a file path or memory buffer.
             * @return True if the importer can read the file, false otherwise.
             */
            virtual bool canRead(const ImporterInputVariant& inputVariant) = 0;

            /**
             * @brief Imports an asset from a file.
             *
             * This method should be overridden by the derived class to do the actual import.
             * @warning Implementation MUST set the main asset data using AssetImporterContext::setMainAssetData() before return
             *
             * @param ctx The context for the import.
             */
            virtual void importImpl(AssetImporterContext& ctx) = 0;

            /**
             * @brief Imports an asset by wrapping a call to the asset importer implementation.
             *
             * This non-overridable method invokes importImpl() to perform the actual asset import. It catches
             * any exceptions thrown during the process, logging an error message that includes the file path from
             * the context. After importImpl() returns, it also verifies that the main asset data has been set in the
             * provided context; if not, it logs an error.
             *
             * Derived classes should override importImpl() to implement custom asset importing behavior.
             *
             * @param ctx The asset importer context containing import parameters and storage for the main asset.
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
